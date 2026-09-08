#pragma once
#include "Util/Joaat.hpp"

#include <comdef.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <functional>
#include <map>
#include <windows.h>
#include <wrl/client.h>


#define REL(o)       \
	o->Release();    \
	if (o)           \
	{                \
		o = nullptr; \
	}

namespace Stand
{
	using namespace Microsoft::WRL;
	using WindowProcedureCallback = std::function<void(HWND, UINT, WPARAM, LPARAM)>;
	// Runs after this frame's D3D12 resource setup is done, so a callback
	// can record its own D3D12 draw calls (the DirectXTK12-based
	// Stand-style renderer in src/Rendering/, the sole draw pipeline this
	// project has now - see GridRenderer.hpp's own class comment) into
	// the same per-frame command list.
	using Direct3DDrawCallBack = std::function<void(ID3D12GraphicsCommandList*)>;

	struct FrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		ID3D12Resource* Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE Descriptor;
		UINT64 FenceValue;
	};

	class Renderer final
	{
	private:
		Renderer();

	public:
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		static void Destroy()
		{
			GetInstance().DestroyImpl();
		}
		static bool Init()
		{
			return GetInstance().InitImpl();
		}

		/**
		 * @brief Add a callback function to handle Windows WindowProcedure
		 *
		 * @param callback Callback function
		 */
		static void AddWindowProcedureCallback(WindowProcedureCallback&& callback)
		{
			GetInstance().AddWindowProcedureCallbackImpl(std::move(callback));
		}

		/**
		 * @brief Add a callback function to record raw D3D12 draw calls
		 * (e.g. DirectXTK12) into this frame's own command list.
		 *
		 * @param callback Callback function
		 * @param priority Low values will be drawn before higher values.
		 * @return true Successfully added callback.
		 * @return false Duplicate render priority was given.
		 */
		static bool AddDirect3DDrawCallBack(Direct3DDrawCallBack&& callback, std::uint32_t priority)
		{
			return GetInstance().AddDirect3DDrawCallBackImpl(std::move(callback), priority);
		}

		static ID3D12Device* GetDevice()
		{
			return GetInstance().m_Device.Get();
		}

		static ID3D12CommandQueue* GetCommandQueue()
		{
			return GetInstance().m_CommandQueue.Get();
		}

		static UINT GetBufferCount()
		{
			return GetInstance().m_SwapChainDesc.BufferCount;
		}

		static void DX12OnPresent()
		{
			GetInstance().DX12OnPresentImpl();
		}

		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			return GetInstance().WndProcImpl(hwnd, msg, wparam, lparam);
		}

		static void WaitForLastFrame();
		static void WaitForNextFrame();

		static void DX12PreResize();
		static void DX12PostResize();

		static bool IsResizing()
		{
			return GetInstance().m_Resizing;
		}

		static void SetResizing(const bool& status)
		{
			GetInstance().m_Resizing = status;
		}

		static void SetSafeToRender()
		{
			GetInstance().m_SafeToRender = true;
		}

	private:
		static void DX12EndFrame();

	private:
		void DestroyImpl();
		bool InitImpl();

		bool InitDX12();

		bool AddDirect3DDrawCallBackImpl(Direct3DDrawCallBack&& callback, std::uint32_t priority);
		void AddWindowProcedureCallbackImpl(WindowProcedureCallback&& callback);

		void DX12OnPresentImpl();

		LRESULT WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		static Renderer& GetInstance()
		{
			static Renderer i{};

			return i;
		}

	private:
		bool m_Initialized;
		bool m_Resizing;
		bool m_SafeToRender;

		//DX12
		std::vector<FrameContext> m_FrameContext;

		DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
		ComPtr<IDXGISwapChain1> m_GameSwapChain;
		ComPtr<IDXGISwapChain3> m_SwapChain;
		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		ComPtr<ID3D12DescriptorHeap> m_BackbufferDescriptorHeap;

		ComPtr<ID3D12Fence> m_Fence;
		HANDLE m_FenceEvent{};
		UINT64 m_FenceLastSignaledValue{};
		HANDLE m_SwapchainWaitableObject{};
		UINT64 m_FrameIndex{};

	private:
		//Other
		std::map<joaat_t, Direct3DDrawCallBack> m_Direct3DDrawCallBacks;
		std::vector<WindowProcedureCallback> m_WindowProcedureCallbacks;
	};
}
