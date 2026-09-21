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
	// Mirrors the texture slots from Stand's own Renderer.hpp/Renderer.cpp
	// (origin/dev), adapted for D3D12 + DirectXTK12 instead of D3D11.
	// Each slot maps to a named PNG in %APPDATA%\StandEnhanced\Theme\.
	// Missing files are silently skipped — callers check IsLoaded(slot)
	// and fall back to DrawRect geometry when the slot is empty.
	enum class IconSlot : int
	{
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
		// Sidebar tab icons — loaded from Theme/Tabs/<name>.png
		TabSelf,
		TabVehicle,
		TabTeleport,
		TabNetwork,
		TabPlayers,
		TabWorld,
		TabRecovery,
		TabSettings,
		TabDebug,
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

		// Spawns a background thread that loads all named PNGs from
		// %APPDATA%\StandEnhanced\Theme\ into D3D12 shader-resource-view
		// slots. Safe to call multiple times (e.g. on reload button).
		static void Load();

		// Reload from disk — same as Load(), exposed with a clearer name
		// for CommandReloadTextures::onClick().
		static void Reload() { Load(); }

		// Returns true if the named slot successfully loaded a texture.
		static bool IsLoaded(IconSlot slot)
		{
			return GetInstance().IsLoadedImpl(slot);
		}

		// Queues a sprite draw at H-space coordinates (same virtual 1920x1080
		// canvas GridItems use). Called from GridItem::draw() during the
		// geometry pass; flushed in FlushQueue() after that pass closes.
		// No-ops if the slot is not loaded.
		static void QueueDraw(IconSlot slot, float x, float y, float size,
		    const DirectX::XMFLOAT4& tint = {1.f, 1.f, 1.f, 1.f})
		{
			GetInstance().QueueDrawImpl(slot, x, y, size, tint);
		}

		// Flushes all queued draws in a single SpriteBatch pass. Call once
		// per frame after m_Batch->End() and before the font SpriteBatch.
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

		mutable std::mutex m_Mutex;
		Slot m_Slots[static_cast<int>(IconSlot::Count)];

		std::vector<DrawCall> m_Queue;

		ID3D12Device* m_SpriteBatchDevice = nullptr;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	};
}
