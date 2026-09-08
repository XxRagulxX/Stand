#pragma once
#include "Rendering/Theme.hpp"

#include <CommonStates.h>
#include <Effects.h>
#include <GraphicsMemory.h>
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <VertexTypes.h>
#include <d3d12.h>
#include <memory>
#include <windows.h>
#include <wrl/client.h>

namespace Stand::Rendering
{
	// First slice of the Stand-style custom renderer port: a DirectXTK12-backed
	// draw-primitives layer that records into the same per-frame D3D12 command
	// list the existing ImGui menu already uses (wired up via
	// Renderer::AddDirect3DDrawCallBack), rather than replacing it.
	//
	// Modeled after stand-reference's src/Rendering/Renderer.hpp/.cpp, which
	// is built on DirectXTK (D3D11) - this is the DirectXTK12/D3D12 equivalent,
	// since this project's swapchain is D3D12.
	//
	// This provides pixel-space DrawRect()/DrawText() primitives (Stand's
	// drawRectH/drawTextC equivalents) that Grid/GridItem-based widgets draw
	// themselves with. Text uses the same embedded "Be Vietnam Pro" spritefont
	// blob stand-reference itself ships (see font_bevietnamprolight.hpp).
	class GridRenderer final
	{
	private:
		GridRenderer() = default;

	public:
		~GridRenderer() = default;

		GridRenderer(const GridRenderer&) = delete;
		GridRenderer(GridRenderer&&) noexcept = delete;
		GridRenderer& operator=(const GridRenderer&) = delete;
		GridRenderer& operator=(GridRenderer&&) noexcept = delete;

		// Registers our draw/window-procedure callbacks with Renderer. Call
		// once from Main(), after Renderer::Init().
		static void Init();

		// Rebuilds the top-level MenuGrid's own header/sidebar next
		// frame (see Grid::invalidate()'s own doc comment) - called by
		// CommandTabs/CommandTabsPos (Commands/Settings/CommandTabs.cpp)
		// whenever either changes Theme::kTabsVisible/kTabsPosition, the
		// same "onChange -> trigger a re-layout" idiom real Stand's own
		// CommandTabs/CommandTabsPos use against g_menu_grid.update().
		static void InvalidateMenuLayout();

		// Draws a solid-colour rectangle at an H-space position/size
		// (top-left origin, Y down - Stand's own virtual 1920x1080 HUD
		// canvas, same as every GridItem's own x/y/width/height and every
		// Theme.hpp geometry constant), converting to real client pixels
		// and then clip space (NDC) internally - see the block comment by
		// PosH2C/SizeH2C in GridRenderer.cpp. Only valid to call while a
		// batch is open, i.e. from a GridItem::Draw() invoked via Grid::
		// Draw() during our own Direct3DDrawCallBack.
		static void DrawRect(float x, float y, float width, float height, const DirectX::XMFLOAT4& colour)
		{
			GetInstance().DrawRectImpl(x, y, width, height, colour);
		}

		// Draws a UTF-8 text string at an H-space position (top-left
		// origin - see DrawRect's own doc comment above). Only valid to
		// call from a GridItem::DrawText() invoked via Grid::DrawText(),
		// i.e. the separate SpriteBatch pass in DrawImpl. No-ops silently
		// if the embedded font failed to load.
		static void DrawText(float x, float y, const char* text, const DirectX::XMFLOAT4& colour, float scale = Theme::kTextScale)
		{
			GetInstance().DrawTextImpl(x, y, text, colour, scale);
		}

		// Measures a UTF-8 text string in H-space units (x = width,
		// y = height - see DrawRect's own doc comment above), for layout
		// purposes (centering, tab widths, ...) against GridItem's own
		// x/y/width/height. Returns {0, 0} if the embedded font failed to
		// load. Safe to call any time (does not require a batch to be
		// open).
		static DirectX::XMFLOAT2 MeasureText(const char* text, float scale = Theme::kTextScale)
		{
			return GetInstance().MeasureTextImpl(text, scale);
		}

		// Screen-space counterparts of DrawRect/DrawText, for ESP (and
		// anything else projecting a 3D world position onto the screen
		// via GET_SCREEN_COORD_FROM_WORLD_COORD) - deliberately separate
		// from DrawRect/DrawText rather than an overload: those take
		// H-space coordinates and go through PosH2C/SizeH2C's own
		// hudCorrection letterboxing (right for menu/HUD content meant
		// to stay inside Stand's virtual 16:9 canvas), but a world-space
		// projection already targets the real screen directly and would
		// be shifted wrong by that same letterboxing on a non-16:9
		// display - these take real client pixels (top-left origin) and
		// skip H2C entirely instead. Same "only valid while a batch is
		// open" rule as DrawRect/DrawText above; DrawLineScreen shares
		// DrawRect's own PrimitiveBatch (drawn as a thin quad along the
		// line for real thickness control, not a 1px GPU line primitive -
		// see GridRenderer.cpp's own DrawLineScreenImpl), so it's valid
		// in the same window DrawRect is. scale for DrawTextScreen is a
		// plain multiplier on the embedded font's own native size - no
		// automatic resolution scaling the way DrawText's own scale
		// parameter gets (see DrawTextImpl's GetResolutionTextScale) -
		// callers already work in real screen pixels, so there's no
		// separate "virtual canvas" resolution to correct for.
		static void DrawLineScreen(float x1, float y1, float x2, float y2, const DirectX::XMFLOAT4& colour, float thickness = 1.5f)
		{
			GetInstance().DrawLineScreenImpl(x1, y1, x2, y2, colour, thickness);
		}

		static void DrawTextScreen(float x, float y, const char* text, const DirectX::XMFLOAT4& colour, float scale = 1.f)
		{
			GetInstance().DrawTextScreenImpl(x, y, text, colour, scale);
		}

		// Screen-space filled rect between two corner points (rather
		// than DrawRect's origin+size), for AutoDriveHUD's own
		// screen-space panel/entity-icon drawing - same PrimitiveBatch,
		// same "valid while a batch is open" rule as DrawRect/
		// DrawLineScreen above.
		static void DrawRectFilledScreen(float x1, float y1, float x2, float y2, const DirectX::XMFLOAT4& colour)
		{
			GetInstance().DrawRectFilledScreenImpl(x1, y1, x2, y2, colour);
		}

		// Screen-space filled convex polygon (fan-triangulated around
		// points[0]) - only valid for a convex point set, same
		// restriction ImGui's own AddConvexPolyFilled has (AutoDriveHUD's
		// vehicle/cone icons are the only current callers, both convex
		// by construction).
		static void DrawPolygonFilledScreen(const DirectX::XMFLOAT2* points, int count, const DirectX::XMFLOAT4& colour)
		{
			GetInstance().DrawPolygonFilledScreenImpl(points, count, colour);
		}

		// Screen-space filled circle, approximated as a regular polygon
		// (segments sides) via DrawPolygonFilledScreen above - no true
		// curve rendering in this pipeline, same "simplest thing that
		// still looks right" trade-off as everywhere else in this system
		// without a real vector-graphics layer.
		static void DrawCircleFilledScreen(float centerX, float centerY, float radius, const DirectX::XMFLOAT4& colour, int segments = 16)
		{
			GetInstance().DrawCircleFilledScreenImpl(centerX, centerY, radius, colour, segments);
		}

	private:
		static GridRenderer& GetInstance()
		{
			static GridRenderer i{};
			return i;
		}

		void DrawImpl(ID3D12GraphicsCommandList* commandList);
		void WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		void DrawRectImpl(float x, float y, float width, float height, const DirectX::XMFLOAT4& colour);
		void DrawTextImpl(float x, float y, const char* text, const DirectX::XMFLOAT4& colour, float scale);
		DirectX::XMFLOAT2 MeasureTextImpl(const char* text, float scale) const;
		void DrawLineScreenImpl(float x1, float y1, float x2, float y2, const DirectX::XMFLOAT4& colour, float thickness);
		void DrawTextScreenImpl(float x, float y, const char* text, const DirectX::XMFLOAT4& colour, float scale);
		void DrawRectFilledScreenImpl(float x1, float y1, float x2, float y2, const DirectX::XMFLOAT4& colour);
		void DrawPolygonFilledScreenImpl(const DirectX::XMFLOAT2* points, int count, const DirectX::XMFLOAT4& colour);
		void DrawCircleFilledScreenImpl(float centerX, float centerY, float radius, const DirectX::XMFLOAT4& colour, int segments);

		void EnsureDeviceResources(ID3D12Device* device);
		void ReleaseDeviceResources();

	private:
		ID3D12Device* m_Device{};

		std::unique_ptr<DirectX::GraphicsMemory> m_GraphicsMemory;
		std::unique_ptr<DirectX::CommonStates> m_States;
		std::unique_ptr<DirectX::BasicEffect> m_Effect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_Batch;

		// Text (only created if the embedded font loads successfully -
		// DrawTextImpl no-ops otherwise, DrawRect/the rest of the pipeline
		// is unaffected).
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_FontDescriptorHeap;
		std::unique_ptr<DirectX::SpriteFont> m_Font;
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	};
}
