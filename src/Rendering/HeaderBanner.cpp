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

		std::thread([this, folder = std::move(folder), generation]() {
			std::vector<std::filesystem::path> files;
			std::error_code ec;
			for (auto& entry : std::filesystem::directory_iterator(folder, ec))
			{
				if (entry.is_regular_file())
					files.push_back(entry.path());
			}

			// Same as real Stand's own loadHeader() - only sorts when
			// there's more than one file (a single custom header needs
			// no ordering).
			if (files.size() > 1)
				std::sort(files.begin(), files.end(), IsFileNameLess);

			if (files.empty())
				return;

			auto* device = Renderer::GetDevice();
			if (!device)
			{
				LOG(WARNING) << "[HeaderBanner] No D3D12 device yet, dropping header load";
				return;
			}

			D3D12_DESCRIPTOR_HEAP_DESC heapDesc{
			    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			    static_cast<UINT>(files.size()),
			    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
			if (const auto result = device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)heap.ReleaseAndGetAddressOf());
			    result < 0)
			{
				LOGF(WARNING, "[HeaderBanner] Failed to create descriptor heap with result: [{}]", result);
				return;
			}

			const auto descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			std::vector<Frame> frames;
			frames.reserve(files.size());

			try
			{
				DirectX::ResourceUploadBatch upload(device);
				upload.Begin();

				for (auto& file : files)
				{
					Frame frame;
					if (FAILED(DirectX::CreateWICTextureFromFile(device, upload, file.c_str(), frame.Texture.ReleaseAndGetAddressOf())))
					{
						LOGF(WARNING, "[HeaderBanner] Failed to load header image: {}", file.string());
						continue;
					}

					const auto desc = frame.Texture->GetDesc();
					frame.Width = static_cast<UINT>(desc.Width);
					frame.Height = desc.Height;

					frame.CpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
					frame.CpuHandle.ptr += frames.size() * descriptorSize;
					frame.GpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
					frame.GpuHandle.ptr += frames.size() * descriptorSize;

					device->CreateShaderResourceView(frame.Texture.Get(), nullptr, frame.CpuHandle);
					frames.push_back(std::move(frame));
				}

				upload.End(Renderer::GetCommandQueue()).wait();
			}
			catch (const std::exception& e)
			{
				LOGF(WARNING, "[HeaderBanner] Failed to load header images: {}", e.what());
				return;
			}

			if (frames.empty())
				return;

			std::lock_guard lock(m_Mutex);

			// A newer LoadFromFolder()/Clear() call landed while this
			// thread was still working - discard this result rather
			// than clobbering whatever the newer call already set.
			if (generation != m_LoadGeneration.load())
				return;

			m_Frames = std::move(frames);
			m_DescriptorHeap = heap;
			m_CurrentFrame = 0;
			m_LastFrameMs = 0;
			m_MsPassed = 0;
			m_Loaded = true;
		}).detach();
	}

	void HeaderBanner::ClearImpl()
	{
		++m_LoadGeneration;

		std::lock_guard lock(m_Mutex);
		m_Frames.clear();
		m_DescriptorHeap.Reset();
		m_CurrentFrame = 0;
		m_LastFrameMs = 0;
		m_MsPassed = 0;
		m_Loaded = false;
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

		if (m_CurrentFrame >= m_Frames.size())
			m_CurrentFrame = 0;

		commandList->SetDescriptorHeaps(1, m_DescriptorHeap.GetAddressOf());

		m_SpriteBatch->SetViewport(viewport);
		m_SpriteBatch->Begin(commandList);

		auto& frame = m_Frames[m_CurrentFrame];
		const RECT dest{static_cast<LONG>(x), static_cast<LONG>(y), static_cast<LONG>(x + width), static_cast<LONG>(y + height)};
		m_SpriteBatch->Draw(frame.GpuHandle, DirectX::XMUINT2{frame.Width, frame.Height}, dest);

		m_SpriteBatch->End();

		// Same animation-timer shape real Stand's own
		// GridItemHeaderAnimation::draw() uses.
		const auto now = static_cast<int64_t>(GetTickCount64());
		if (m_LastFrameMs != 0)
			m_MsPassed += now - m_LastFrameMs;
		m_LastFrameMs = now;

		while (m_MsPassed >= kFrameIntervalMs)
		{
			m_MsPassed -= kFrameIntervalMs;
			m_CurrentFrame = (m_CurrentFrame + 1) % m_Frames.size();
		}
	}
}
