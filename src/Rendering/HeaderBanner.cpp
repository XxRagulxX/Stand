#include "Rendering/HeaderBanner.hpp"
#include "Rendering/Renderer.hpp"

#include <ResourceUploadBatch.h>
#include <RenderTargetState.h>
#include <WICTextureLoader.h>

#include <algorithm>
#include <mutex>
#include <thread>

namespace Stand::Rendering
{
	namespace
	{
		bool IsFileNameLess(const std::filesystem::path& a, const std::filesystem::path& b)
		{
			return a.filename().string() < b.filename().string();
		}

		std::mutex s_UploadMutex;
	}

	void HeaderBanner::LoadFromFileImpl(std::filesystem::path filePath)
	{
		const auto generation = ++m_LoadGeneration;

		std::thread([this, filePath = std::move(filePath), generation]() {
			if (generation != m_LoadGeneration.load())
				return;

			if (!std::filesystem::is_regular_file(filePath))
				return;

			auto* device = Renderer::GetDevice();
			if (!device)
				return;

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

				if (FAILED(DirectX::CreateWICTextureFromFile(device, upload, filePath.c_str(), frame.Texture.ReleaseAndGetAddressOf())))
					return;

				const auto desc = frame.Texture->GetDesc();
				frame.Width     = static_cast<UINT>(desc.Width);
				frame.Height    = desc.Height;
				frame.CpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
				frame.GpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
				device->CreateShaderResourceView(frame.Texture.Get(), nullptr, frame.CpuHandle);

				{
					std::lock_guard uploadLock(s_UploadMutex);
					if (generation != m_LoadGeneration.load())
						return;
					upload.End(Renderer::GetCommandQueue()).wait();
				}
			}
			catch (...)
			{
				return;
			}

			if (generation != m_LoadGeneration.load())
				return;

			auto result          = std::make_shared<PendingResult>();
			result->generation   = generation;
			result->frames.push_back(std::move(frame));
			result->heap         = std::move(heap);
			m_Pending.store(std::move(result));
			HeaderBanner::kFrameCount.store(1);
			m_Loaded             = true;
		}).detach();
	}

	void HeaderBanner::LoadFromFolderImpl(std::filesystem::path folder)
	{
		const auto generation = ++m_LoadGeneration;

		std::thread([this, folder = std::move(folder), generation]() {
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

			if (generation != m_LoadGeneration.load())
				return;

			auto* device = Renderer::GetDevice();
			if (!device)
				return;

			const UINT n        = static_cast<UINT>(files.size());
			const UINT descSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			D3D12_DESCRIPTOR_HEAP_DESC heapDesc{
			    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, n,
			    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
			if (FAILED(device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)heap.ReleaseAndGetAddressOf())))
				return;

			std::vector<Frame> frames(n);
			try
			{
				DirectX::ResourceUploadBatch upload(device);
				upload.Begin();

				D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
				D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heap->GetGPUDescriptorHandleForHeapStart();

				for (size_t i = 0; i < n; ++i)
				{
					if (generation != m_LoadGeneration.load())
						return;

					auto& frame = frames[i];
					if (FAILED(DirectX::CreateWICTextureFromFile(device, upload, files[i].c_str(), frame.Texture.ReleaseAndGetAddressOf())))
					{
						cpuHandle.ptr += descSize;
						gpuHandle.ptr += descSize;
						continue;
					}
					const auto desc = frame.Texture->GetDesc();
					frame.Width     = static_cast<UINT>(desc.Width);
					frame.Height    = desc.Height;
					frame.CpuHandle = cpuHandle;
					frame.GpuHandle = gpuHandle;
					device->CreateShaderResourceView(frame.Texture.Get(), nullptr, cpuHandle);
					cpuHandle.ptr += descSize;
					gpuHandle.ptr += descSize;
				}

				{
					std::lock_guard uploadLock(s_UploadMutex);
					if (generation != m_LoadGeneration.load())
						return;
					upload.End(Renderer::GetCommandQueue()).wait();
				}
			}
			catch (...)
			{
				return;
			}

			frames.erase(
				std::remove_if(frames.begin(), frames.end(), [](const Frame& f) { return !f.Texture; }),
				frames.end());

			if (frames.empty())
				return;

			if (generation != m_LoadGeneration.load())
				return;

			auto result        = std::make_shared<PendingResult>();
			result->generation = generation;
			result->frames     = std::move(frames);
			result->heap       = std::move(heap);
			const size_t frameCount = result->frames.size();
			m_Pending.store(std::move(result));
			HeaderBanner::kFrameCount.store(frameCount);
			m_Loaded           = true;
		}).detach();
	}

	void HeaderBanner::ClearImpl()
	{
		++m_LoadGeneration;
		kFrameCount.store(0);
		m_Pending.store(nullptr);

		std::lock_guard lock(m_Mutex);
		m_Frames.clear();
		m_DescriptorHeap.Reset();
		m_CurrentFrame = 0;
		m_LastFrameMs  = 0;
		m_MsPassed     = 0;
		m_Loaded       = false;
	}

	float HeaderBanner::GetRenderHeightImpl(float width) const
	{
		std::lock_guard lock(m_Mutex);

		if (!m_Frames.empty())
		{
			const auto& frame = m_Frames[m_CurrentFrame < m_Frames.size() ? m_CurrentFrame : 0];
			if (frame.Width == 0)
				return 0.f;
			return static_cast<float>(frame.Height) * (width / static_cast<float>(frame.Width));
		}

		if (auto pending = m_Pending.load())
		{
			if (!pending->frames.empty())
			{
				const auto& frame = pending->frames[0];
				if (frame.Width == 0)
					return 0.f;
				return static_cast<float>(frame.Height) * (width / static_cast<float>(frame.Width));
			}
		}

		return 0.f;
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
		catch (...)
		{
			m_SpriteBatch.reset();
		}
	}

	void HeaderBanner::DrawImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport, float x, float y, float width, float height)
	{
		auto* device = Renderer::GetDevice();
		if (!device)
			return;

		std::lock_guard lock(m_Mutex);

		if (auto pending = m_Pending.exchange(nullptr))
		{
			if (pending->generation == m_LoadGeneration.load())
			{
				m_Frames         = std::move(pending->frames);
				m_DescriptorHeap = std::move(pending->heap);
				m_CurrentFrame   = 0;
				m_LastFrameMs    = 0;
				m_MsPassed       = 0;
			}
		}

		if (m_Frames.empty() || !m_DescriptorHeap)
			return;

		EnsureSpriteBatch(device);
		if (!m_SpriteBatch)
			return;

		const auto now = static_cast<int64_t>(GetTickCount64());
		if (m_LastFrameMs != 0)
			m_MsPassed += now - m_LastFrameMs;
		m_LastFrameMs = now;

		if (m_Frames.size() > 1)
		{
			if (m_MsPassed > m_FrameIntervalMs)
				m_MsPassed = m_FrameIntervalMs;

			while (m_MsPassed >= m_FrameIntervalMs)
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
