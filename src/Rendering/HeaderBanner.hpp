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
	// Ported from real Stand's own "Custom Header" feature (Commands/
	// Widgets/CommandHeader.hpp/.cpp on origin/stand-reference) - a
	// local-image-folder banner drawn above the menu's own addressbar,
	// minus everything that requires stand.sh network access (the ~26
	// curated named headers and their animated-download variants) -
	// see CommandHeader.cpp for that scope decision.
	//
	// Stand's own TextureArray (Rendering/TextureBase.hpp/.cpp there)
	// is a D3D11 concept; this is this project's own D3D12-native
	// equivalent, built on the same DirectXTK12 primitives GridRenderer
	// already uses for its own SpriteFont text (EnsureDeviceResources/
	// DrawImpl there) - a separate descriptor heap and SpriteBatch of
	// its own rather than sharing GridRenderer's font heap, since that
	// heap is sized for exactly one descriptor (the font) and grows
	// only with the number of header frames actually loaded, which
	// isn't known until a folder is scanned.
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

		// Scans folder for regular files (alphanumeric-sorted if more
		// than one, same as real Stand's own alphanum_less-sorted
		// loadHeader(files) call) and loads each as a frame, off the
		// render thread - same "don't block a frame on disk I/O" reason
		// real Stand's own loadHeader() runs via
		// Exceptional::createManagedExceptionalThread. Safe to call
		// again with a different folder; a load already in flight is
		// left to finish (its result is simply discarded once the newer
		// one lands - see the .cpp for why silently dropping this edge
		// case is an acceptable trade-off here).
		static void LoadFromFolder(std::filesystem::path folder)
		{
			GetInstance().LoadFromFolderImpl(std::move(folder));
		}

		// Stops drawing and releases every loaded frame - real Stand's
		// own g_renderer.header_state = HIDE (CommandHeader.cpp's own
		// value == 0 branch).
		static void Clear()
		{
			GetInstance().ClearImpl();
		}

		static bool IsLoaded()
		{
			return GetInstance().m_Loaded.load();
		}

		// Height (H-space) for a banner drawn at H-space width, aspect-
		// locked to the current frame's own texture - real Stand's own
		// TextureBase::getRenderHeight(). Returns 0 if nothing is loaded.
		static float GetRenderHeight(float width)
		{
			return GetInstance().GetRenderHeightImpl(width);
		}

		// Draws the current frame and advances the animation timer -
		// same "advance as part of drawing" shape real Stand's own
		// GridItemHeaderAnimation::draw() has. x/y/width/height are
		// real client pixels (already H2C-converted by the caller -
		// see GridRenderer::DrawImpl), not H-space. Only valid to call
		// once per frame, from the same command list GridRenderer's own
		// font SpriteBatch pass uses - opens/closes its own SpriteBatch
		// scope with its own descriptor heap bound, so it can be called
		// right alongside that pass without either one disturbing the
		// other's bound heap.
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

		// Real Stand's own g_renderer.header_speed default (32ms/frame)
		// is exposed as a separate command (CommandHeaderAnimationSpeed)
		// this port skips (see CommandHeader.cpp's own comment) - a
		// fixed, still-reasonable interval instead.
		static constexpr int64_t kFrameIntervalMs = 100;

		std::atomic<bool> m_Loaded{false};
		std::atomic<uint64_t> m_LoadGeneration{0};

		ID3D12Device* m_SpriteBatchDevice = nullptr;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	};
}
