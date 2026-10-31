#pragma once
#include <SpriteBatch.h>
#include <d3d12.h>
#include <wrl/client.h>

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

namespace Stand::Rendering
{
	enum class IconSlot : int
	{
		// ── Core toggle / list icons
		ToggleOff = 0,
		ToggleOn,
		ToggleOffAuto,
		ToggleOnAuto,
		ToggleOffList,
		ToggleOnList,
		List,
		Link,
		Enabled,
		Disabled,
		User,
		Friends,
		Users,
		Edit,
		Search,
		HeaderLoading,

		// ── Player / network badge icons 
		WantedStar,
		Lock,
		Rs,
		RsVerified,
		RsCreated,

		// ── Utility icons
		Blankbox,
		Newline,
		Reset,

		// ── Unicode emoji icons (chat / notifications)
		Uni0000,
		Uni26A0,
		Uni2728,
		Uni2764,
		Uni1F4AF,
		Uni1F60A,
		Uni1F480,
		Uni1F525,
		Uni1F602,
		Uni1F629,
		Uni1F633,

		// ── Sidebar tab icons — loaded from Theme/Tabs/<name>.png
		TabSelf,
		TabVehicle,
		TabTeleport,
		TabNetwork,
		TabPlayers,
		TabWorld,
		TabRecovery,
		TabSettings,
		TabDebug,

		// ── Custom slots — loaded from Theme/Custom/<hex>.png
		Custom00,
		Custom01,
		Custom02,
		Custom03,
		Custom04,
		Custom05,
		Custom06,
		Custom07,
		Custom08,
		Custom09,
		Custom0A,
		Custom0B,
		Custom0C,
		Custom0D,
		Custom0E,
		Custom0F,
		Custom10,
		Custom11,
		Custom12,
		Custom13,
		Custom14,
		Custom15,
		Custom16,
		Custom17,
		Custom18,
		Custom19,
		Custom1A,
		Custom1B,
		Custom1C,
		Custom1D,
		Custom1E,
		Custom1F,

		Count,
	};

	class ThemeIcons final
	{
	private:
		ThemeIcons() = default;

	public:
		ThemeIcons(const ThemeIcons&) = delete;
		ThemeIcons(ThemeIcons&&) noexcept = delete;
		ThemeIcons& operator=(const ThemeIcons&) = delete;
		ThemeIcons& operator=(ThemeIcons&&) noexcept = delete;

		static void Load();
		static void Reload() { Load(); }

		static bool IsLoaded(IconSlot slot)
		{
			return GetInstance().IsLoadedImpl(slot);
		}

		static void QueueDraw(IconSlot slot, float x, float y, float size,
		    const DirectX::XMFLOAT4& tint = {1.f, 1.f, 1.f, 1.f})
		{
			GetInstance().QueueDrawImpl(slot, x, y, size, tint);
		}

		static void FlushQueue(ID3D12GraphicsCommandList* commandList,
		    const D3D12_VIEWPORT& viewport)
		{
			GetInstance().FlushQueueImpl(commandList, viewport);
		}

	private:
		static ThemeIcons& GetInstance()
		{
			static ThemeIcons i{};
			return i;
		}

		struct Slot
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> Texture;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Heap;
			UINT Width = 0;
			UINT Height = 0;
			D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{};
			D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle{};
			bool Loaded = false;
		};

		struct DrawCall
		{
			int slot;
			float x, y, size;
			DirectX::XMFLOAT4 tint;
		};

		void LoadImpl();
		bool IsLoadedImpl(IconSlot slot) const;
		void QueueDrawImpl(IconSlot slot, float x, float y, float size, const DirectX::XMFLOAT4& tint);
		void FlushQueueImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport);
		void EnsureSpriteBatch(ID3D12Device* device);
		bool LoadSlot(int idx, const std::wstring& path);
		bool LoadSlotFromMemory(int idx, const uint8_t* data, size_t size);
		void CopySlot(int dst, int src);

		mutable std::mutex m_Mutex;
		Slot m_Slots[static_cast<int>(IconSlot::Count)];

		std::vector<DrawCall> m_Queue;

		ID3D12Device* m_SpriteBatchDevice = nullptr;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	};
}
