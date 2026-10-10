#include "Rendering/StandPort/BackgroundBlur.hpp"

#include <ResourceUploadBatch.h>
#include <RenderTargetState.h>

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand::Rendering
{
	void BackgroundBlur::DrawH(float x, float y, float width, float height, uint8_t passes)
	{
		const auto posC  = GridRenderer::PosH2C(x, y);
		const auto sizeC = GridRenderer::SizeH2C(width, height);
		DrawC(posC.x, posC.y, sizeC.x, sizeC.y, passes);
	}

	void BackgroundBlur::DrawC(float cx, float cy, float cw, float ch, uint8_t passes)
	{
		if (cx < 0.f || cy < 0.f || cw <= 0.f || ch <= 0.f || passes == 0)
			return;

		const auto clientSize = GridRenderer::GetClientSize();
		if (cx >= clientSize.x || cy >= clientSize.y)
			return;

		const UINT x = static_cast<UINT>(cx);
		const UINT y = static_cast<UINT>(cy);
		const UINT w = static_cast<UINT>(cw);
		const UINT h = static_cast<UINT>(ch);

		if (x + w > static_cast<UINT>(clientSize.x) || y + h > static_cast<UINT>(clientSize.y))
			return;
		if (w == 0 || h == 0)
			return;

		auto* cmdList = GridRenderer::GetActiveCommandList();
		auto* device  = Renderer::GetDevice();
		auto* backBuf = Renderer::GetCurrentBackBuffer();
		if (!cmdList || !device || !backBuf)
			return;

		const DXGI_FORMAT fmt = backBuf->GetDesc().Format;

		EnsureResources(device, w, h, fmt);
		if (!m_Capture || !m_Blur || !m_PostProcess || !m_SpriteBatch)
			return;

		GridRenderer::PauseBatch();

		// Transition back buffer: RENDER_TARGET → COPY_SOURCE
		{
			D3D12_RESOURCE_BARRIER b{};
			b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			b.Transition.pResource   = backBuf;
			b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			b.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			b.Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_SOURCE;
			cmdList->ResourceBarrier(1, &b);
		}

		// Copy the requested region from back buffer into capture texture
		{
			D3D12_TEXTURE_COPY_LOCATION dst{};
			dst.pResource        = m_Capture.Get();
			dst.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dst.SubresourceIndex = 0;

			D3D12_TEXTURE_COPY_LOCATION src{};
			src.pResource        = backBuf;
			src.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			src.SubresourceIndex = 0;

			D3D12_BOX box{x, y, 0, x + w, y + h, 1};
			cmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, &box);
		}

		// Transition back buffer: COPY_SOURCE → RENDER_TARGET
		{
			D3D12_RESOURCE_BARRIER b{};
			b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			b.Transition.pResource   = backBuf;
			b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			b.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
			b.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
			cmdList->ResourceBarrier(1, &b);
		}

		// Restore back buffer as the active render target
		const auto backBufRtv = Renderer::GetCurrentBackBufferRtv();
		cmdList->OMSetRenderTargets(1, &backBufRtv, FALSE, nullptr);

		// Transition capture: COPY_DEST → PIXEL_SHADER_RESOURCE
		{
			D3D12_RESOURCE_BARRIER b{};
			b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			b.Transition.pResource   = m_Capture.Get();
			b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			b.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			b.Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			cmdList->ResourceBarrier(1, &b);
		}

		// --- Blur ping-pong ---
		const UINT srvInc = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		const UINT rtvInc = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_GPU_DESCRIPTOR_HANDLE captureSrvGpu = m_SrvHeap->GetGPUDescriptorHandleForHeapStart();
		D3D12_GPU_DESCRIPTOR_HANDLE blurSrvGpu    = {captureSrvGpu.ptr + srvInc};
		D3D12_CPU_DESCRIPTOR_HANDLE captureRtv    = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
		D3D12_CPU_DESCRIPTOR_HANDLE blurRtv       = {captureRtv.ptr + rtvInc};

		D3D12_VIEWPORT blurVp{0.f, 0.f, static_cast<float>(w), static_cast<float>(h), 0.f, 1.f};
		D3D12_RECT     blurSc{0, 0, static_cast<LONG>(w), static_cast<LONG>(h)};

		cmdList->SetDescriptorHeaps(1, m_SrvHeap.GetAddressOf());
		cmdList->RSSetViewports(1, &blurVp);
		cmdList->RSSetScissorRects(1, &blurSc);

		// Initial states: capture=PSR, blur=RT
		// captureCurrent=true means capture holds the current source data
		bool captureCurrent = true;

		for (uint8_t i = 0; i < passes; ++i)
		{
			if (captureCurrent)
			{
				if (i > 0)
				{
					D3D12_RESOURCE_BARRIER bs[2]{};
					bs[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
					bs[0].Transition.pResource   = m_Capture.Get();
					bs[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
					bs[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
					bs[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
					bs[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
					bs[1].Transition.pResource   = m_Blur.Get();
					bs[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
					bs[1].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
					bs[1].Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
					cmdList->ResourceBarrier(2, bs);
				}
				cmdList->OMSetRenderTargets(1, &blurRtv, FALSE, nullptr);
				m_PostProcess->SetSourceTexture(captureSrvGpu, m_Capture.Get());
				m_PostProcess->Process(cmdList);
				captureCurrent = false;
			}
			else
			{
				D3D12_RESOURCE_BARRIER bs[2]{};
				bs[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				bs[0].Transition.pResource   = m_Blur.Get();
				bs[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				bs[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
				bs[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				bs[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				bs[1].Transition.pResource   = m_Capture.Get();
				bs[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				bs[1].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				bs[1].Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
				cmdList->ResourceBarrier(2, bs);
				cmdList->OMSetRenderTargets(1, &captureRtv, FALSE, nullptr);
				m_PostProcess->SetSourceTexture(blurSrvGpu, m_Blur.Get());
				m_PostProcess->Process(cmdList);
				captureCurrent = true;
			}
		}

		// Transition the result texture from RENDER_TARGET to PSR for drawing
		ID3D12Resource*          resultTex = captureCurrent ? m_Capture.Get() : m_Blur.Get();
		D3D12_GPU_DESCRIPTOR_HANDLE resultSrv = captureCurrent ? captureSrvGpu : blurSrvGpu;
		{
			D3D12_RESOURCE_BARRIER b{};
			b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			b.Transition.pResource   = resultTex;
			b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			b.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			b.Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			cmdList->ResourceBarrier(1, &b);
		}

		// Draw the blurred result at the original position on the back buffer
		const auto fullVp = GridRenderer::GetActiveViewport();
		D3D12_RECT fullSc{0, 0, static_cast<LONG>(fullVp.Width), static_cast<LONG>(fullVp.Height)};
		cmdList->OMSetRenderTargets(1, &backBufRtv, FALSE, nullptr);
		cmdList->RSSetViewports(1, &fullVp);
		cmdList->RSSetScissorRects(1, &fullSc);

		cmdList->SetDescriptorHeaps(1, m_SrvHeap.GetAddressOf());
		m_SpriteBatch->SetViewport(fullVp);
		m_SpriteBatch->Begin(cmdList);
		{
			const RECT dest{static_cast<LONG>(x), static_cast<LONG>(y),
			    static_cast<LONG>(x + w), static_cast<LONG>(y + h)};
			m_SpriteBatch->Draw(resultSrv, DirectX::XMUINT2{w, h}, dest);
		}
		m_SpriteBatch->End();

		// Reset resource states for next frame:
		// After SpriteBatch: result is PSR, other is PSR (source of last blur pass)
		{
			D3D12_RESOURCE_BARRIER bs[2]{};
			bs[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			bs[0].Transition.pResource   = m_Capture.Get();
			bs[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			bs[0].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			bs[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_DEST;
			bs[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			bs[1].Transition.pResource   = m_Blur.Get();
			bs[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			bs[1].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			bs[1].Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
			cmdList->ResourceBarrier(2, bs);
		}

		GridRenderer::ResumeBatch();
	}

	void BackgroundBlur::EnsureResources(ID3D12Device* device, UINT w, UINT h, DXGI_FORMAT format)
	{
		if (m_Device == device && m_Width == w && m_Height == h && m_Format == format
		    && m_Capture && m_Blur && m_SrvHeap && m_RtvHeap && m_PostProcess && m_SpriteBatch)
			return;

		m_Capture.Reset();
		m_Blur.Reset();
		m_SrvHeap.Reset();
		m_RtvHeap.Reset();
		m_PostProcess.reset();
		m_SpriteBatch.reset();
		m_Device = device;
		m_Width  = w;
		m_Height = h;
		m_Format = format;

		auto createTex = [&](Microsoft::WRL::ComPtr<ID3D12Resource>& tex,
		    D3D12_RESOURCE_STATES initState,
		    D3D12_RESOURCE_FLAGS flags) -> bool
		{
			D3D12_RESOURCE_DESC desc{};
			desc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Width            = w;
			desc.Height           = h;
			desc.DepthOrArraySize = 1;
			desc.MipLevels        = 1;
			desc.Format           = format;
			desc.SampleDesc       = {1, 0};
			desc.Flags            = flags;

			D3D12_CLEAR_VALUE clearVal{};
			clearVal.Format   = format;

			D3D12_HEAP_PROPERTIES heap{D3D12_HEAP_TYPE_DEFAULT};
			return SUCCEEDED(device->CreateCommittedResource(
			    &heap, D3D12_HEAP_FLAG_NONE, &desc, initState,
			    (flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) ? &clearVal : nullptr,
			    __uuidof(ID3D12Resource), (void**)tex.ReleaseAndGetAddressOf()));
		};

		if (!createTex(m_Capture, D3D12_RESOURCE_STATE_COPY_DEST,
		    D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET))
			return;

		if (!createTex(m_Blur, D3D12_RESOURCE_STATE_RENDER_TARGET,
		    D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET))
			return;

		// SRV heap (2 slots, shader-visible): slot 0 = capture, slot 1 = blur
		{
			D3D12_DESCRIPTOR_HEAP_DESC hd{D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2,
			    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};
			if (FAILED(device->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap),
			    (void**)m_SrvHeap.ReleaseAndGetAddressOf())))
				return;
		}

		// RTV heap (2 slots): slot 0 = capture, slot 1 = blur
		{
			D3D12_DESCRIPTOR_HEAP_DESC hd{D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2,
			    D3D12_DESCRIPTOR_HEAP_FLAG_NONE};
			if (FAILED(device->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap),
			    (void**)m_RtvHeap.ReleaseAndGetAddressOf())))
				return;
		}

		const UINT srvInc = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		const UINT rtvInc = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Create SRVs
		device->CreateShaderResourceView(m_Capture.Get(), nullptr,
		    {m_SrvHeap->GetCPUDescriptorHandleForHeapStart().ptr});
		device->CreateShaderResourceView(m_Blur.Get(), nullptr,
		    {m_SrvHeap->GetCPUDescriptorHandleForHeapStart().ptr + srvInc});

		// Create RTVs
		device->CreateRenderTargetView(m_Capture.Get(), nullptr,
		    {m_RtvHeap->GetCPUDescriptorHandleForHeapStart().ptr});
		device->CreateRenderTargetView(m_Blur.Get(), nullptr,
		    {m_RtvHeap->GetCPUDescriptorHandleForHeapStart().ptr + rtvInc});

		// BasicPostProcess (GaussianBlur_5x5) — render target format matches blur textures
		try
		{
			DirectX::RenderTargetState rtState(format, DXGI_FORMAT_UNKNOWN);
			m_PostProcess = std::make_unique<DirectX::BasicPostProcess>(
			    device, rtState, DirectX::BasicPostProcess::GaussianBlur_5x5);
		}
		catch (...)
		{
			m_PostProcess.reset();
			return;
		}

		// SpriteBatch for drawing the final blurred result onto the back buffer
		try
		{
			DirectX::RenderTargetState rtState(format, DXGI_FORMAT_UNKNOWN);
			DirectX::SpriteBatchPipelineStateDescription pd(rtState);
			DirectX::ResourceUploadBatch upload(device);
			upload.Begin();
			m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(device, upload, pd);
			upload.End(Renderer::GetCommandQueue()).wait();
		}
		catch (...)
		{
			m_SpriteBatch.reset();
		}
	}

	void BackgroundBlur::Reset()
	{
		m_Capture.Reset();
		m_Blur.Reset();
		m_SrvHeap.Reset();
		m_RtvHeap.Reset();
		m_PostProcess.reset();
		m_SpriteBatch.reset();
		m_Width  = 0;
		m_Height = 0;
		m_Format = DXGI_FORMAT_UNKNOWN;
		m_Device = nullptr;
	}
}
