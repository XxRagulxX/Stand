#pragma once
#include <SpriteBatch.h>
#include <d3d12.h>
#include <wrl/client.h>

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>

namespace Stand::Rendering
{
	class HeaderLoadingSprite final
	{
	private:
		HeaderLoadingSprite() = default;

	public:
		~HeaderLoadingSprite() = default;

		HeaderLoadingSprite(const HeaderLoadingSprite&) = delete;
		HeaderLoadingSprite(HeaderLoadingSprite&&) noexcept = delete;
		HeaderLoadingSprite& operator=(const HeaderLoadingSprite&) = delete;
		HeaderLoadingSprite& operator=(HeaderLoadingSprite&&) noexcept = delete;

		static void LoadFromFile(std::filesystem::path filePath)
		{
			GetInstance().LoadFromFileImpl(std::move(filePath));
		}

		static void Clear()
		{
			GetInstance().ClearImpl();
		}

		static bool IsLoaded()
		{
			return GetInstance().m_Loaded.load();
		}

		static float GetRenderHeight(float width)
		{
			return GetInstance().GetRenderHeightImpl(width);
		}

		static void Draw(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport,
		    float x, float y, float width, float height)
		{
			GetInstance().DrawImpl(commandList, viewport, x, y, width, height);
		}

	private:
		static HeaderLoadingSprite& GetInstance()
		{
			static HeaderLoadingSprite i{};
			return i;
		}

		struct Frame
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> Texture;
			UINT Width  = 0;
			UINT Height = 0;
			D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{};
			D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle{};
		};

		void LoadFromFileImpl(std::filesystem::path filePath);
		void ClearImpl();
		float GetRenderHeightImpl(float width) const;
		void DrawImpl(ID3D12GraphicsCommandList* commandList, const D3D12_VIEWPORT& viewport,
		    float x, float y, float width, float height);
		void EnsureSpriteBatch(ID3D12Device* device);

		mutable std::mutex m_Mutex;
		Frame m_Frame;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;

		std::atomic<bool> m_Loaded{false};
		std::atomic<uint64_t> m_LoadGeneration{0};

		ID3D12Device* m_SpriteBatchDevice = nullptr;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	};
}
