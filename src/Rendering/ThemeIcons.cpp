#include "Rendering/ThemeIcons.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Theme.hpp"
#include "Core/FileMgr.hpp"

#include <ResourceUploadBatch.h>
#include <RenderTargetState.h>
#include <WICTextureLoader.h>

#include <cstdlib>
#include <filesystem>
#include <thread>

namespace Stand::Rendering
{
	namespace
	{
		// Matches Stand's own slot-to-filename table in Renderer.cpp
		// (origin/dev, reloadTextures()). File extension (.png) is appended
		// by LoadSlot() below — same pattern as Stand's own RELOAD_TEXTURE macro.
		static constexpr const wchar_t* kSlotNames[static_cast<int>(IconSlot::Count)] = {
		    L"Toggle Off",
		    L"Toggle On",
		    L"Toggle Off Auto",
		    L"Toggle On Auto",
		    L"Toggle Off List",
		    L"Toggle On List",
		    L"List",
		    L"Link",
		    L"Enabled",
		    L"Disabled",
		    L"User",
		    L"Friends",
		    L"Users",
		    L"Edit",
		    L"Search",
		    L"Header Loading",
		    // Tab icons — path is relative to theme root (Tabs/<name>.png)
		    L"Tabs/Self",
		    L"Tabs/Vehicle",
		    L"Tabs/Teleport",
		    L"Tabs/Network",
		    L"Tabs/Players",
		    L"Tabs/World",
		    L"Tabs/Recovery",
		    L"Tabs/Settings",
		    L"Tabs/Debug",
		};

		// PosH2C/SizeH2C replicated from GridRenderer.cpp — converts
		// Stand's virtual 1920x1080 H-space coords to real client pixels.
		DirectX::XMFLOAT2 SizeH2C(float x, float y, float clientW, float clientH)
		{
			const float hudW = Theme::kHudWidth;
			const float hudH = Theme::kHudHeight;
			float corrX = 0.f, corrY = 0.f;
			const float expectedW = clientH * (16.f / 9.f);
			if (clientW > expectedW)
				corrX = (clientW - expectedW) * 0.5f;
			else
				corrY = (clientH - clientW * (9.f / 16.f)) * 0.5f;
			return {(x / hudW) * (clientW - corrX * 2.f), (y / hudH) * (clientH - corrY * 2.f)};
		}

		DirectX::XMFLOAT2 PosH2C(float x, float y, float clientW, float clientH)
		{
			float corrX = 0.f, corrY = 0.f;
			const float expectedW = clientH * (16.f / 9.f);
			if (clientW > expectedW)
				corrX = (clientW - expectedW) * 0.5f;
			else
				corrY = (clientH - clientW * (9.f / 16.f)) * 0.5f;
			const auto s = SizeH2C(x, y, clientW, clientH);
			return {s.x + corrX, s.y + corrY};
		}
	}

	void ThemeIcons::Load()
	{
		std::thread([]{ GetInstance().LoadImpl(); }).detach();
	}

	void ThemeIcons::LoadImpl()
	{
		auto* device = Renderer::GetDevice();
		if (!device)
			return;

		// Primary theme folder: %APPDATA%\StandEnhanced\Theme
		const auto themeDir = FileMgr::GetProjectFolder("Theme").Path();
		std::filesystem::create_directories(themeDir);
		std::filesystem::create_directories(themeDir / "Tabs");
		std::filesystem::create_directories(themeDir / "Custom");

		// Fallback 1: StandEnhanced\Themes (old plural name the Open Theme
		// Folder button used before it was corrected to "Theme").
		const auto themesDir = themeDir.parent_path() / "Themes";

		// Fallback 2: %APPDATA%\Stand\Theme (original Stand installation).
		std::filesystem::path standThemeDir;
		if (const char* appdata = std::getenv("APPDATA"))
			standThemeDir = std::filesystem::path(appdata) / "Stand" / "Theme";

		for (int i = 0; i < static_cast<int>(IconSlot::Count); ++i)
		{
			// Priority: Theme/ → Themes/ → Stand/Theme/
			if (LoadSlot(i, (themeDir / kSlotNames[i]).wstring() + L".png"))
				continue;
			if (LoadSlot(i, (themesDir / kSlotNames[i]).wstring() + L".png"))
				continue;
			if (!standThemeDir.empty())
				LoadSlot(i, (standThemeDir / kSlotNames[i]).wstring() + L".png");
		}
	}

	bool ThemeIcons::LoadSlot(int idx, const std::wstring& path)
	{
		auto* device = Renderer::GetDevice();
		if (!device)
			return false;

		if (!std::filesystem::is_regular_file(path))
			return false;

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{
		    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1,
		    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
		if (FAILED(device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)heap.ReleaseAndGetAddressOf())))
			return false;

		Slot slot;
		try
		{
			DirectX::ResourceUploadBatch upload(device);
			upload.Begin();
			if (FAILED(DirectX::CreateWICTextureFromFile(device, upload, path.c_str(), slot.Texture.ReleaseAndGetAddressOf())))
				return false;
			const auto desc = slot.Texture->GetDesc();
			slot.Width  = static_cast<UINT>(desc.Width);
			slot.Height = desc.Height;
			slot.CpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
			slot.GpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
			device->CreateShaderResourceView(slot.Texture.Get(), nullptr, slot.CpuHandle);
			upload.End(Renderer::GetCommandQueue()).wait();
		}
		catch (...) { return false; }

		slot.Heap   = std::move(heap);
		slot.Loaded = true;

		std::lock_guard lock(m_Mutex);
		m_Slots[idx] = std::move(slot);
		return true;
	}

	bool ThemeIcons::IsLoadedImpl(IconSlot slot) const
	{
		const int idx = static_cast<int>(slot);
		if (idx < 0 || idx >= static_cast<int>(IconSlot::Count))
			return false;
		std::lock_guard lock(m_Mutex);
		return m_Slots[idx].Loaded;
	}

	void ThemeIcons::QueueDrawImpl(IconSlot slot, float x, float y, float size, const DirectX::XMFLOAT4& tint)
	{
		const int idx = static_cast<int>(slot);
		{
			std::lock_guard lock(m_Mutex);
			if (idx < 0 || idx >= static_cast<int>(IconSlot::Count) || !m_Slots[idx].Loaded)
				return;
		}
		m_Queue.push_back({idx, x, y, size, tint});
	}

	void ThemeIcons::EnsureSpriteBatch(ID3D12Device* device)
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
		catch (...) { m_SpriteBatch.reset(); }
	}

	void ThemeIcons::FlushQueueImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport)
	{
		if (m_Queue.empty())
			return;

		auto* device = Renderer::GetDevice();
		if (!device)
		{
			m_Queue.clear();
			return;
		}

		EnsureSpriteBatch(device);
		if (!m_SpriteBatch)
		{
			m_Queue.clear();
			return;
		}

		const float clientW = viewport.Width;
		const float clientH = viewport.Height;

		// Each slot has its own descriptor heap — bind and draw per-slot.
		// Group consecutive same-slot draws to minimise heap switches.
		std::lock_guard lock(m_Mutex);

		int prevSlot = -1;
		for (auto& dc : m_Queue)
		{
			auto& s = m_Slots[dc.slot];
			if (!s.Loaded || s.Width == 0 || s.Height == 0)
				continue;

			if (dc.slot != prevSlot)
			{
				if (prevSlot != -1)
					m_SpriteBatch->End();

				commandList->SetDescriptorHeaps(1, s.Heap.GetAddressOf());
				m_SpriteBatch->SetViewport(viewport);
				m_SpriteBatch->Begin(commandList);
				prevSlot = dc.slot;
			}

			const auto posC  = PosH2C(dc.x, dc.y, clientW, clientH);
			const auto sizeC = SizeH2C(dc.size, dc.size, clientW, clientH);

			const RECT dest{
			    static_cast<LONG>(posC.x), static_cast<LONG>(posC.y),
			    static_cast<LONG>(posC.x + sizeC.x), static_cast<LONG>(posC.y + sizeC.y)};

			const DirectX::XMVECTORF32 colour = {dc.tint.x, dc.tint.y, dc.tint.z, dc.tint.w};
			m_SpriteBatch->Draw(s.GpuHandle, DirectX::XMUINT2{s.Width, s.Height}, dest, colour);
		}

		if (prevSlot != -1)
			m_SpriteBatch->End();

		m_Queue.clear();
	}
}
