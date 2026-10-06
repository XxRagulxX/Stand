#include "Rendering/HeaderLoadingSprite.hpp"
#include "Rendering/Renderer.hpp"

#include <ResourceUploadBatch.h>
#include <RenderTargetState.h>
#include <WICTextureLoader.h>

#include <thread>


namespace Stand::Rendering
{
	void HeaderLoadingSprite::LoadFromFileImpl(std::filesystem::path filePath)
	{
		const auto generation = ++m_LoadGeneration;

		std::thread([this, filePath = std::move(filePath), generation]()
		{
			if (generation != m_LoadGeneration.load()) return;

			auto* device = Renderer::GetDevice();
			if (!device)
				return;

			if (!std::filesystem::is_regular_file(filePath))
			{
				LOGF(WARNING, "[HeaderLoadingSprite] File not found: {}", filePath.string());
				return;
			}

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
				{
					LOGF(WARNING, "[HeaderLoadingSprite] Failed to load: {}", filePath.string());
					return;
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
				LOGF(WARNING, "[HeaderLoadingSprite] Exception loading {}: {}", filePath.string(), e.what());
				return;
			}

			if (generation != m_LoadGeneration.load()) return;

			std::lock_guard lock(m_Mutex);
			m_Frame          = std::move(frame);
			m_DescriptorHeap = std::move(heap);
			m_Loaded         = true;
		}).detach();
	}

	void HeaderLoadingSprite::ClearImpl()
	{
		++m_LoadGeneration;
		std::lock_guard lock(m_Mutex);
		m_Frame = {};
		m_DescriptorHeap.Reset();
		m_Loaded = false;
	}

	float HeaderLoadingSprite::GetRenderHeightImpl(float width) const
	{
		std::lock_guard lock(m_Mutex);
		if (m_Frame.Width == 0)
			return 0.f;
		return static_cast<float>(m_Frame.Height) * (width / static_cast<float>(m_Frame.Width));
	}

	void HeaderLoadingSprite::EnsureSpriteBatch(ID3D12Device* device)
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
			LOGF(WARNING, "[HeaderLoadingSprite] Failed to create sprite batch: {}", e.what());
			m_SpriteBatch.reset();
		}
	}

	void HeaderLoadingSprite::DrawImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport,
	    float x, float y, float width, float height)
	{
		std::lock_guard lock(m_Mutex);
		if (!m_Frame.Texture || !m_DescriptorHeap)
			return;

		auto* device = Renderer::GetDevice();
		if (!device)
			return;

		EnsureSpriteBatch(device);
		if (!m_SpriteBatch)
			return;

		commandList->SetDescriptorHeaps(1, m_DescriptorHeap.GetAddressOf());
		m_SpriteBatch->SetViewport(viewport);
		m_SpriteBatch->Begin(commandList);

		const RECT dest{
		    static_cast<LONG>(x),         static_cast<LONG>(y),
		    static_cast<LONG>(x + width), static_cast<LONG>(y + height)};
		m_SpriteBatch->Draw(m_Frame.GpuHandle, DirectX::XMUINT2{m_Frame.Width, m_Frame.Height}, dest);

		m_SpriteBatch->End();
	}
}
