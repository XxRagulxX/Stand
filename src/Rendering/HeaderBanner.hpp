#pragma once
#include <SpriteBatch.h>
#include <d3d12.h>
#include <wrl/client.h>

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <vector>

namespace Stand::Rendering
{
	class HeaderBanner final
	{
	private:
		HeaderBanner() = default;

	public:
		~HeaderBanner() = default;

		HeaderBanner(const HeaderBanner&) = delete;
		HeaderBanner(HeaderBanner&&) noexcept = delete;
		HeaderBanner& operator=(const HeaderBanner&) = delete;
		HeaderBanner& operator=(HeaderBanner&&) noexcept = delete;

		static void LoadFromFolder(std::filesystem::path folder)
		{
			GetInstance().LoadFromFolderImpl(std::move(folder));
		}

		static void Clear()
		{
			GetInstance().ClearImpl();
		}

		static void LoadFromFile(std::filesystem::path filePath)
		{
			GetInstance().LoadFromFileImpl(std::move(filePath));
		}

		static bool IsLoaded()
		{
			return GetInstance().m_Loaded.load();
		}

		static inline uint16_t kLoadingProgress = 0;
		static inline uint16_t kLoadingGoal = 0;
		static inline std::atomic<size_t> kFrameCount{0};

		static float GetRenderHeight(float width)
		{
			return GetInstance().GetRenderHeightImpl(width);
		}

		static void SetFrameIntervalMs(int64_t ms)
		{
			GetInstance().m_FrameIntervalMs = ms;
		}

		static int64_t GetFrameIntervalMs()
		{
			return GetInstance().m_FrameIntervalMs;
		}

		static void SetBgBlur(bool on)
		{
			GetInstance().m_BgBlur = on;
		}

		static bool GetBgBlur()
		{
			return GetInstance().m_BgBlur;
		}

		static void Draw(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport, float x, float y, float width, float height)
		{
			GetInstance().DrawImpl(commandList, viewport, x, y, width, height);
		}

	private:
		static HeaderBanner& GetInstance()
		{
			static HeaderBanner i{};
			return i;
		}

		struct Frame
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> Texture;
			UINT Width = 0;
			UINT Height = 0;
			D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{};
			D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle{};
		};

		// Uploaded on the background thread, swapped into active on the render thread in DrawImpl.
		struct PendingResult
		{
			std::vector<Frame> frames;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
			uint64_t generation = 0;
		};

		void LoadFromFolderImpl(std::filesystem::path folder);
		void LoadFromFileImpl(std::filesystem::path filePath);
		void ClearImpl();
		float GetRenderHeightImpl(float width) const;
		void DrawImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport, float x, float y, float width, float height);
		void EnsureSpriteBatch(ID3D12Device* device);

		mutable std::mutex m_Mutex;
		std::vector<Frame> m_Frames;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;
		size_t m_CurrentFrame = 0;
		int64_t m_LastFrameMs = 0;
		int64_t m_MsPassed = 0;

		int64_t m_FrameIntervalMs = 32;
		bool m_BgBlur = false;

		std::atomic<bool> m_Loaded{false};
		std::atomic<uint64_t> m_LoadGeneration{0};

		// Set by background thread (after GPU upload), consumed by render thread in DrawImpl.
		std::atomic<std::shared_ptr<PendingResult>> m_Pending{nullptr};

		ID3D12Device* m_SpriteBatchDevice = nullptr;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	};
}
