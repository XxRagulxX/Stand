#include "Rendering/HeaderBanner.hpp"
#include "Rendering/Renderer.hpp"

#include <ResourceUploadBatch.h>
#include <RenderTargetState.h>
#include <WICTextureLoader.h>

#include <algorithm>
#include <thread>

namespace Stand::Rendering
{
	namespace
	{
		bool IsFileNameLess(const std::filesystem::path& a, const std::filesystem::path& b)
		{
			return a.filename().string() < b.filename().string();
		}
	}

	void HeaderBanner::LoadFromFolderImpl(std::filesystem::path folder)
	{
		const auto generation = ++m_LoadGeneration;
		m_PendingReady.store(false);
		m_PendingLoading.store(false);

		std::thread([this, folder = std::move(folder), generation]() {
			// Scan and sort — same ordering as real Stand's own loadHeader().
			std::vector<std::filesystem::path> files;
			std::error_code ec;
			for (auto& entry : std::filesystem::directory_iterator(folder, ec))
			{
				if (entry.is_regular_file())
					files.push_back(entry.path());
			}
			if (files.size() > 1)
				std::sort(files.begin(), files.end(), IsFileNameLess);
			if (files.empty())
				return;

			{
				std::lock_guard lock(m_Mutex);
				if (generation != m_LoadGeneration.load()) return;
				m_FramePaths = files;
				m_StreamIdx  = 0;
				m_PendingIdx = files.size() > 1 ? 1 : 0;
				m_Streaming  = true;
				m_Loaded     = false;
			}

			if (!LoadFrameIntoCurrent(0, files[0], generation))
				return;

			// Kick pre-load of frame 1 in a separate thread.
			if (files.size() > 1 && !m_PendingLoading.exchange(true))
			{
				auto nextPath = files[1];
				std::thread([this, nextPath = std::move(nextPath), generation]()
				{
					LoadFrameIntoPending(1, nextPath, generation);
					m_PendingLoading.store(false);
				}).detach();
			}
		}).detach();
	}

	bool HeaderBanner::LoadFrameIntoCurrent(size_t idx, const std::filesystem::path& path, uint64_t generation)
	{
		auto* device = Renderer::GetDevice();
		if (!device)
		{
			LOG(WARNING) << "[HeaderBanner] No D3D12 device yet, dropping header load";
			return false;
		}

		if (!std::filesystem::is_regular_file(path))
		{
			LOGF(WARNING, "[HeaderBanner] File not found: {}", path.string());
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{
		    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1,
		    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
		if (FAILED(device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)heap.ReleaseAndGetAddressOf())))
			return false;

		Frame frame;
		try
		{
			DirectX::ResourceUploadBatch upload(device);
			upload.Begin();
			if (FAILED(DirectX::CreateWICTextureFromFile(device, upload, path.c_str(), frame.Texture.ReleaseAndGetAddressOf())))
			{
				LOGF(WARNING, "[HeaderBanner] Failed to load: {}", path.string());
				return false;
			}
			const auto desc = frame.Texture->GetDesc();
			frame.Width     = static_cast<UINT>(desc.Width);
			frame.Height    = desc.Height;
			frame.CpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
			frame.GpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
			device->CreateShaderResourceView(frame.Texture.Get(), nullptr, frame.CpuHandle);
			upload.End(Renderer::GetCommandQueue()).wait();
		}
		catch (const std::exception& e)
		{
			LOGF(WARNING, "[HeaderBanner] Exception loading {}: {}", path.string(), e.what());
			return false;
		}

		std::lock_guard lock(m_Mutex);
		if (generation != m_LoadGeneration.load()) return false;

		m_Frames        = { std::move(frame) };
		m_DescriptorHeap = std::move(heap);
		m_CurrentFrame  = 0;
		m_StreamIdx     = idx;
		m_LastFrameMs   = 0;
		m_MsPassed      = 0;
		m_Loaded        = true;
		return true;
	}

	void HeaderBanner::LoadFrameIntoPending(size_t idx, const std::filesystem::path& path, uint64_t generation)
	{
		if (generation != m_LoadGeneration.load()) return;

		auto* device = Renderer::GetDevice();
		if (!device || !std::filesystem::is_regular_file(path)) return;

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{
		    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1,
		    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
		if (FAILED(device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)heap.ReleaseAndGetAddressOf())))
			return;

		Frame frame;
		try
		{
			DirectX::ResourceUploadBatch upload(device);
			upload.Begin();
			if (FAILED(DirectX::CreateWICTextureFromFile(device, upload, path.c_str(), frame.Texture.ReleaseAndGetAddressOf())))
				return;
			const auto desc = frame.Texture->GetDesc();
			frame.Width     = static_cast<UINT>(desc.Width);
			frame.Height    = desc.Height;
			frame.CpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
			frame.GpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
			device->CreateShaderResourceView(frame.Texture.Get(), nullptr, frame.CpuHandle);
			upload.End(Renderer::GetCommandQueue()).wait();
		}
		catch (...) { return; }

		if (generation != m_LoadGeneration.load()) return;

		{
			std::lock_guard pLock(m_PendingMutex);
			m_PendingFrame = std::move(frame);
			m_PendingHeap  = std::move(heap);
		}
		m_PendingReady.store(true, std::memory_order_release);
	}

	void HeaderBanner::LoadFromFileImpl(std::filesystem::path filePath)
	{
		const auto generation = ++m_LoadGeneration;
		m_PendingReady.store(false);
		m_PendingLoading.store(false);

		std::thread([this, filePath = std::move(filePath), generation]() {
			// Single-image preset: non-streaming, no cycling.
			{
				std::lock_guard lock(m_Mutex);
				if (generation != m_LoadGeneration.load()) return;
				m_Streaming = false;
				m_FramePaths.clear();
			}
			LoadFrameIntoCurrent(0, filePath, generation);
		}).detach();
	}

	void HeaderBanner::ClearImpl()
	{
		++m_LoadGeneration;
		m_PendingReady.store(false);

		{
			std::lock_guard pLock(m_PendingMutex);
			m_PendingFrame = {};
			m_PendingHeap.Reset();
		}

		std::lock_guard lock(m_Mutex);
		m_Frames.clear();
		m_DescriptorHeap.Reset();
		for (int i = 0; i < kStaleSlots; ++i) { m_StaleFrames[i] = {}; m_StaleHeaps[i].Reset(); }
		m_StaleRingIdx = 0;
		m_FramePaths.clear();
		m_Streaming    = false;
		m_StreamIdx    = 0;
		m_PendingIdx   = 0;
		m_CurrentFrame = 0;
		m_LastFrameMs  = 0;
		m_MsPassed     = 0;
		m_Loaded       = false;
	}

	float HeaderBanner::GetRenderHeightImpl(float width) const
	{
		std::lock_guard lock(m_Mutex);
		if (m_Frames.empty())
			return 0.f;

		const auto& frame = m_Frames[m_CurrentFrame < m_Frames.size() ? m_CurrentFrame : 0];
		if (frame.Width == 0)
			return 0.f;

		return static_cast<float>(frame.Height) * (width / static_cast<float>(frame.Width));
	}

	void HeaderBanner::EnsureSpriteBatch(ID3D12Device* device)
	{
		if (m_SpriteBatch && m_SpriteBatchDevice == device)
			return;

		m_SpriteBatch.reset();
		m_SpriteBatchDevice = device;

		try
		{
			DirectX::RenderTargetState rtState(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
			DirectX::SpriteBatchPipelineStateDescription spritePd(rtState);

			DirectX::ResourceUploadBatch upload(device);
			upload.Begin();

			m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(device, upload, spritePd);

			upload.End(Renderer::GetCommandQueue()).wait();
		}
		catch (const std::exception& e)
		{
			LOGF(WARNING, "[HeaderBanner] Failed to create sprite batch: {}", e.what());
			m_SpriteBatch.reset();
		}
	}

	void HeaderBanner::DrawImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport, float x, float y, float width, float height)
	{
		std::lock_guard lock(m_Mutex);
		if (m_Frames.empty() || !m_DescriptorHeap)
			return;

		auto* device = Renderer::GetDevice();
		if (!device)
			return;

		EnsureSpriteBatch(device);
		if (!m_SpriteBatch)
			return;

		// ── Timer ───────────────────────────────────────────────────────────
		const auto now = static_cast<int64_t>(GetTickCount64());
		if (m_LastFrameMs != 0)
			m_MsPassed += now - m_LastFrameMs;
		m_LastFrameMs = now;

		if (m_Streaming && !m_FramePaths.empty())
		{
			if (m_MsPassed > m_FrameIntervalMs)
				m_MsPassed = m_FrameIntervalMs;

			if (m_MsPassed >= m_FrameIntervalMs &&
			    m_PendingReady.load(std::memory_order_acquire))
			{
				m_MsPassed -= m_FrameIntervalMs;
				m_PendingReady.store(false, std::memory_order_relaxed);

				const int slot = m_StaleRingIdx % kStaleSlots;
				++m_StaleRingIdx;
				{
					std::lock_guard pLock(m_PendingMutex);
					m_StaleFrames[slot] = std::move(m_Frames[0]);
					m_StaleHeaps [slot] = std::move(m_DescriptorHeap);
					m_Frames[0]         = std::move(m_PendingFrame);
					m_DescriptorHeap    = std::move(m_PendingHeap);
				}
				m_StreamIdx = m_PendingIdx;

				m_PendingIdx = (m_StreamIdx + 1) % m_FramePaths.size();
				if (!m_PendingLoading.exchange(true))
				{
					auto       nextPath = m_FramePaths[m_PendingIdx];
					const auto gen      = m_LoadGeneration.load();
					const auto idx      = m_PendingIdx;
					std::thread([this, nextPath = std::move(nextPath), idx, gen]()
					{
						LoadFrameIntoPending(idx, nextPath, gen);
						m_PendingLoading.store(false);
					}).detach();
				}
			}
		}
		else
		{
			while (m_Frames.size() > 1 && m_MsPassed >= m_FrameIntervalMs)
			{
				m_MsPassed -= m_FrameIntervalMs;
				m_CurrentFrame = (m_CurrentFrame + 1) % m_Frames.size();
			}
		}

		if (m_CurrentFrame >= m_Frames.size())
			m_CurrentFrame = 0;

		commandList->SetDescriptorHeaps(1, m_DescriptorHeap.GetAddressOf());
		m_SpriteBatch->SetViewport(viewport);
		m_SpriteBatch->Begin(commandList);

		auto& frame = m_Frames[m_CurrentFrame];
		const RECT dest{
		    static_cast<LONG>(x),         static_cast<LONG>(y),
		    static_cast<LONG>(x + width), static_cast<LONG>(y + height)};
		m_SpriteBatch->Draw(frame.GpuHandle, DirectX::XMUINT2{frame.Width, frame.Height}, dest);

		m_SpriteBatch->End();
	}
}
