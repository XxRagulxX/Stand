#pragma once
#include <PostProcess.h>
#include <SpriteBatch.h>
#include <d3d12.h>
#include <wrl/client.h>

#include <cstdint>
#include <memory>

namespace Stand::Rendering
{
	class BackgroundBlur
	{
	public:
		void DrawH(float x, float y, float width, float height, uint8_t passes);
		void DrawC(float cx, float cy, float cw, float ch, uint8_t passes);
		void Reset();

	private:
		void EnsureResources(ID3D12Device* device, UINT w, UINT h, DXGI_FORMAT format);

		Microsoft::WRL::ComPtr<ID3D12Resource> m_Capture;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_Blur;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SrvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
		std::unique_ptr<DirectX::BasicPostProcess> m_PostProcess;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

		UINT m_Width  = 0;
		UINT m_Height = 0;
		DXGI_FORMAT m_Format = DXGI_FORMAT_UNKNOWN;
		ID3D12Device* m_Device = nullptr;
	};
}
