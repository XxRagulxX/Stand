#include "ThemeIcons.hpp"
#include "ThemeIconsData.hpp"
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
		static constexpr const wchar_t* kSlotNames[static_cast<int>(IconSlot::Count)] = {
		    // Core toggle / list
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
		    // Player / network badges
		    L"Wanted Star",
		    L"Lock",
		    L"Rs",
		    L"Rs Verified",
		    L"Rs Created",
		    // Utility
		    L"Blankbox",
		    L"Newline",
		    L"Reset",
		    // Unicode emoji
		    L"Uni0000",
		    L"Uni26A0",
		    L"Uni2728",
		    L"Uni2764",
		    L"Uni1F4AF",
		    L"Uni1F60A",
		    L"Uni1F480",
		    L"Uni1F525",
		    L"Uni1F602",
		    L"Uni1F629",
		    L"Uni1F633",
		    // Sidebar tab icons
		    L"Tabs/Self",
		    L"Tabs/Vehicle",
		    L"Tabs/Teleport",
		    L"Tabs/Network",
		    L"Tabs/Players",
		    L"Tabs/World",
		    L"Tabs/Recovery",
		    L"Tabs/Settings",
		    L"Tabs/Debug",
		    // Custom slots — Theme/Custom/<hex>.png
		    L"Custom/00",
		    L"Custom/01",
		    L"Custom/02",
		    L"Custom/03",
		    L"Custom/04",
		    L"Custom/05",
		    L"Custom/06",
		    L"Custom/07",
		    L"Custom/08",
		    L"Custom/09",
		    L"Custom/0A",
		    L"Custom/0B",
		    L"Custom/0C",
		    L"Custom/0D",
		    L"Custom/0E",
		    L"Custom/0F",
		    L"Custom/10",
		    L"Custom/11",
		    L"Custom/12",
		    L"Custom/13",
		    L"Custom/14",
		    L"Custom/15",
		    L"Custom/16",
		    L"Custom/17",
		    L"Custom/18",
		    L"Custom/19",
		    L"Custom/1A",
		    L"Custom/1B",
		    L"Custom/1C",
		    L"Custom/1D",
		    L"Custom/1E",
		    L"Custom/1F",
		};

		struct EmbeddedFallback { IconSlot slot; const uint8_t* data; size_t size; };
		static constexpr EmbeddedFallback kEmbedded[] = {
		    { IconSlot::WantedStar, IconData::kWantedStar, IconData::kWantedStarSize },
		    { IconSlot::Lock,       IconData::kLock,       IconData::kLockSize       },
		    { IconSlot::Rs,         IconData::kRs,         IconData::kRsSize         },
		    { IconSlot::RsVerified, IconData::kRsVerified, IconData::kRsVerifiedSize },
		    { IconSlot::RsCreated,  IconData::kRsCreated,  IconData::kRsCreatedSize  },
		    { IconSlot::Blankbox,   IconData::kBlankbox,   IconData::kBlankboxSize   },
		    { IconSlot::Newline,    IconData::kNewline,    IconData::kNewlineSize    },
		    { IconSlot::Reset,      IconData::kReset,      IconData::kResetSize      },
		    { IconSlot::Uni0000,    IconData::kUni0000,    IconData::kUni0000Size    },
		    { IconSlot::Uni26A0,    IconData::kUni26A0,    IconData::kUni26A0Size    },
		    { IconSlot::Uni2728,    IconData::kUni2728,    IconData::kUni2728Size    },
		    { IconSlot::Uni2764,    IconData::kUni2764,    IconData::kUni2764Size    },
		    { IconSlot::Uni1F4AF,   IconData::kUni1F4AF,   IconData::kUni1F4AFSize   },
		    { IconSlot::Uni1F60A,   IconData::kUni1F60A,   IconData::kUni1F60ASize   },
		    { IconSlot::Uni1F480,   IconData::kUni1F480,   IconData::kUni1F480Size   },
		    { IconSlot::Uni1F525,   IconData::kUni1F525,   IconData::kUni1F525Size   },
		    { IconSlot::Uni1F602,   IconData::kUni1F602,   IconData::kUni1F602Size   },
		    { IconSlot::Uni1F629,   IconData::kUni1F629,   IconData::kUni1F629Size   },
		    { IconSlot::Uni1F633,   IconData::kUni1F633,   IconData::kUni1F633Size   },
		};

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
		const auto themeDir = FileMgr::GetProjectFolder("Theme");
		std::filesystem::create_directories(themeDir);
		std::filesystem::create_directories(themeDir / "Tabs");
		std::filesystem::create_directories(themeDir / "Custom");

		const auto themesDir = themeDir.parent_path() / "Themes";

		for (int i = 0; i < static_cast<int>(IconSlot::Count); ++i)
		{
			if (LoadSlot(i, (themeDir / kSlotNames[i]).wstring() + L".png"))
				continue;
			LoadSlot(i, (themesDir / kSlotNames[i]).wstring() + L".png");
		}

		for (const auto& fb : kEmbedded)
		{
			const int idx = static_cast<int>(fb.slot);
			bool loaded = false;
			{ std::lock_guard lock(m_Mutex); loaded = m_Slots[idx].Loaded; }
			if (!loaded)
				LoadSlotFromMemory(idx, fb.data, fb.size);
		}

		const int custom0 = static_cast<int>(IconSlot::Custom00);
		for (int i = 0; i < 32; ++i)
		{
			bool loaded = false;
			{ std::lock_guard lock(m_Mutex); loaded = m_Slots[custom0 + i].Loaded; }
			if (!loaded)
				CopySlot(custom0 + i, static_cast<int>(IconData::kCustomFallback[i]));
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

	bool ThemeIcons::LoadSlotFromMemory(int idx, const uint8_t* data, size_t size)
	{
		auto* device = Renderer::GetDevice();
		if (!device)
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
			if (FAILED(DirectX::CreateWICTextureFromMemory(device, upload, data, size, slot.Texture.ReleaseAndGetAddressOf())))
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

	void ThemeIcons::CopySlot(int dst, int src)
	{
		std::lock_guard lock(m_Mutex);
		if (src >= 0 && src < static_cast<int>(IconSlot::Count) && m_Slots[src].Loaded)
			m_Slots[dst] = m_Slots[src];
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
