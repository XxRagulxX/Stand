#include "Core/DetourHook.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	HRESULT SwapChain::Present(IDXGISwapChain* that, UINT syncInterval, UINT flags)
	{
		if (g_Running && !Renderer::IsResizing())
		{
			Renderer::DX12OnPresent();
		}
		return Hooking::Get<SwapChain::Present>()->Original<decltype(&SwapChain::Present)>()(that, syncInterval, flags);
	}

	HRESULT SwapChain::ResizeBuffers(IDXGISwapChain* that, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	{
		if (g_Running)
		{
			Renderer::DX12PreResize();
			const auto result =
			    Hooking::Get<SwapChain::ResizeBuffers>()->Original<decltype(&SwapChain::ResizeBuffers)>()(that, bufferCount, width, height, newFormat, swapChainFlags);
			Renderer::DX12PostResize();
			return result;
		}
		return Hooking::Get<SwapChain::ResizeBuffers>()->Original<decltype(&SwapChain::ResizeBuffers)>()(that, bufferCount, width, height, newFormat, swapChainFlags);
	}
}