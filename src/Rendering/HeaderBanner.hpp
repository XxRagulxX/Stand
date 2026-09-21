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

		void LoadFromFolderImpl(std::filesystem::path folder);
		void LoadFromFileImpl(std::filesystem::path filePath);
		void ClearImpl();
		float GetRenderHeightImpl(float width) const;
		void DrawImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport, float x, float y, float width, float height);
		void EnsureSpriteBatch(ID3D12Device* device);
		bool LoadFrameIntoCurrent(size_t idx, const std::filesystem::path& path, uint64_t generation);
		void LoadFrameIntoPending(size_t idx, const std::filesystem::path& path, uint64_t generation);

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

		ID3D12Device* m_SpriteBatchDevice = nullptr;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

		std::vector<std::filesystem::path>           m_FramePaths;
		size_t                                       m_StreamIdx    = 0;
		size_t                                       m_PendingIdx   = 0;
		bool                                         m_Streaming    = false;

		mutable std::mutex                           m_PendingMutex;
		Frame                                        m_PendingFrame;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_PendingHeap;
		std::atomic<bool>                            m_PendingReady  {false};
		std::atomic<bool>                            m_PendingLoading{false};

		static constexpr int kStaleSlots = 3;
		Frame                                        m_StaleFrames[kStaleSlots];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_StaleHeaps [kStaleSlots];
		int                                          m_StaleRingIdx = 0;
	};
}
