#include "Rendering/GridRenderer.hpp"

#include "Core/FileMgr.hpp"
#include "Menu/GUI.hpp"
#include "Rendering/AutoDriveHUD.hpp"
#include "Rendering/ChatDisplay.hpp"
#include "Rendering/DescriptionPanel.hpp"
#include "Rendering/ESP.hpp"
#include "Rendering/HeaderBanner.hpp"
#include "Rendering/Onboarding.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuCommandConsole.hpp"
#include "Rendering/MenuFocus.hpp"
#include "Rendering/MenuGrid.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/MenuPopup.hpp"
#include "Core/Pointers.hpp"
#include "Rendering/InputCapture.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Overlay.hpp"
#include "Rendering/SettingsNotificationsGrid.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/font_bevietnamprolight.hpp"

#include <RenderTargetState.h>
#include <ResourceUploadBatch.h>
#include <algorithm>
#include <climits>
#include <cmath>
#include <vector>

namespace Stand::Rendering
{
	namespace
	{
		// GridItem coordinates (and every Theme.hpp geometry constant)
		// are defined in Stand's own virtual 1920x1080 "H" (HUD) canvas,
		// exactly like stand-reference's own GridItem/Renderer classes
		// (see Rendering/Renderer.hpp's client_size/hudCorrectionC and
		// {size,pos}H2C in Renderer.cpp) - NOT literal client pixels,
		// which is what this project treated them as before and is why
		// the menu was both the wrong size and positioned wrong
		// (MenuGrid's own default_origin only makes sense as an H-space
		// coordinate - see the comment by kHeaderX/kHeaderY in
		// MenuGrid.cpp). H2C below converts an H-space coordinate to a
		// real client pixel for drawing. There's deliberately no C2H
		// (OS cursor position -> H-space) any more - this menu is
		// keyboard-only, no mouse, so nothing ever needs to convert a
		// cursor position at all.
		//
		// hudCorrection lets a non-16:9 client area (ultrawide, or a
		// window narrower than it is tall) black-bar the extra space
		// instead of stretching the menu's aspect ratio - same as
		// Stand's own hudCorrectionC. kHudWidth/kHudHeight themselves
		// live in Theme.hpp - MenuGrid.cpp needs the same numbers for
		// its own scroll-window math.
		using Theme::kHudHeight;
		using Theme::kHudWidth;

		DirectX::XMFLOAT2 GetClientSize()
		{
			return {static_cast<float>(*Pointers.ScreenResX), static_cast<float>(*Pointers.ScreenResY)};
		}

		DirectX::XMFLOAT2 GetHudCorrection(const DirectX::XMFLOAT2& clientSize)
		{
			const float expectedWidth = clientSize.y * (16.f / 9.f);
			if (clientSize.x > expectedWidth)
				return {(clientSize.x - expectedWidth) * 0.5f, 0.f};

			const float expectedHeight = clientSize.x * (9.f / 16.f);
			return {0.f, (clientSize.y - expectedHeight) * 0.5f};
		}

		// Stand's own resolution_text_scale - see Theme::kTextScale's own
		// doc comment for why text needs this on top of that fixed scale.
		float GetResolutionTextScale(const DirectX::XMFLOAT2& clientSize)
		{
			return std::min(clientSize.x / kHudWidth, clientSize.y / kHudHeight);
		}

		DirectX::XMFLOAT2 SizeH2C(float x, float y)
		{
			const auto clientSize = GetClientSize();
			const auto correction = GetHudCorrection(clientSize);
			return {(x / kHudWidth) * (clientSize.x - correction.x * 2.f), (y / kHudHeight) * (clientSize.y - correction.y * 2.f)};
		}

		DirectX::XMFLOAT2 PosH2C(float x, float y)
		{
			const auto clientSize = GetClientSize();
			const auto correction = GetHudCorrection(clientSize);
			const auto size = SizeH2C(x, y);
			return {size.x + correction.x, size.y + correction.y};
		}

		// Real client pixel space (top-left origin, Y down) -> clip
		// space / NDC (Y up) - shared by DrawRectImpl and
		// DrawLineScreenImpl (the only two consumers of raw NDC quads;
		// DrawTextImpl/DrawTextScreenImpl hand SpriteFont pixel
		// coordinates directly and never need this).
		DirectX::XMFLOAT3 PixelToNdc(float px, float py)
		{
			const auto clientSize = GetClientSize();
			return {(px / clientSize.x) * 2.f - 1.f, 1.f - (py / clientSize.y) * 2.f, 0.f};
		}
	}

	static MenuGrid g_MenuGrid{};

	void GridRenderer::InvalidateMenuLayout()
	{
		g_MenuGrid.InvalidateLayout();
	}

	void GridRenderer::EnsureDeviceResources(ID3D12Device* device)
	{
		if (m_Device == device && m_Batch)
			return;

		ReleaseDeviceResources();
		m_Device = device;

		// TODO: once more DirectXTK12-based subsystems exist, GraphicsMemory
		// should be owned centrally (one instance per device, Commit()'d once
		// per frame) rather than per-consumer.
		m_GraphicsMemory = std::make_unique<DirectX::GraphicsMemory>(device);
		m_States = std::make_unique<DirectX::CommonStates>(device);

		DirectX::RenderTargetState rtState(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);

		DirectX::EffectPipelineStateDescription pd(&DirectX::VertexPositionColor::InputLayout,
		    DirectX::CommonStates::AlphaBlend,
		    DirectX::CommonStates::DepthNone,
		    DirectX::CommonStates::CullNone,
		    rtState,
		    D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		m_Effect = std::make_unique<DirectX::BasicEffect>(device, DirectX::EffectFlags::VertexColor, pd);
		m_Batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(device);

		LOG(INFO) << "[GridRenderer] DirectXTK12 rect pipeline ready";

		// Text: embedded "Be Vietnam Pro" spritefont (see
		// font_bevietnamprolight.hpp). Failure here (e.g. a malformed blob)
		// is non-fatal - DrawTextImpl no-ops if m_Font/m_SpriteBatch are
		// null, and rect drawing above is unaffected.
		D3D12_DESCRIPTOR_HEAP_DESC fontHeapDesc{
		    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		    1,
		    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};
		if (const auto result = device->CreateDescriptorHeap(&fontHeapDesc,
		        __uuidof(ID3D12DescriptorHeap),
		        (void**)m_FontDescriptorHeap.ReleaseAndGetAddressOf());
		    result < 0)
		{
			LOGF(WARNING, "[GridRenderer] Failed to create font descriptor heap with result: [{}]", result);
		}
		else
		{
			try
			{
				DirectX::ResourceUploadBatch upload(device);
				upload.Begin();

				m_Font = std::make_unique<DirectX::SpriteFont>(device,
				    upload,
				    reinterpret_cast<const uint8_t*>(font_bevietnamprolight::chunk_1),
				    sizeof(font_bevietnamprolight::chunk_1),
				    m_FontDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				    m_FontDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
				m_Font->SetDefaultCharacter(L'?');

				DirectX::SpriteBatchPipelineStateDescription spritePd(rtState);
				m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(device, upload, spritePd);

				upload.End(Renderer::GetCommandQueue()).wait();

				LOG(INFO) << "[GridRenderer] Embedded font loaded, text pipeline ready";
			}
			catch (const std::exception& e)
			{
				LOGF(WARNING, "[GridRenderer] Failed to load embedded font: {}", e.what());
				m_Font.reset();
				m_SpriteBatch.reset();
			}
		}
	}

	void GridRenderer::ReleaseDeviceResources()
	{
		m_SpriteBatch.reset();
		m_Font.reset();
		m_FontDescriptorHeap.Reset();
		m_Batch.reset();
		m_Effect.reset();
		m_States.reset();
		m_GraphicsMemory.reset();
		m_Device = nullptr;
	}

	void GridRenderer::DrawImpl(ID3D12GraphicsCommandList* commandList)
	{
		// No menu/Insert-toggle gate up here any more, unlike before
		// Notifications moved onto this pipeline - toasts have to draw
		// every frame regardless of whether the Stand-style menu is even
		// open (see Notifications.hpp's own class comment), so resource
		// setup and the two batches below always run now; menuActive
		// below is what still gates the menu/popup content specifically.
		if (Renderer::IsResizing())
			return;

		auto device = Renderer::GetDevice();
		if (!device)
			return;

		EnsureDeviceResources(device);

		if (Theme::kFontReloadPending && m_FontDescriptorHeap)
		{
			Theme::kFontReloadPending = false;
			m_Font.reset();
			m_SpriteBatch.reset();
			try
			{
				DirectX::ResourceUploadBatch upload(m_Device);
				upload.Begin();
				if (Theme::kFontPath.empty())
				{
					m_Font = std::make_unique<DirectX::SpriteFont>(m_Device, upload,
					    reinterpret_cast<const uint8_t*>(font_bevietnamprolight::chunk_1),
					    sizeof(font_bevietnamprolight::chunk_1),
					    m_FontDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
					    m_FontDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
				}
				else
				{
					std::wstring wpath(Theme::kFontPath.begin(), Theme::kFontPath.end());
					m_Font = std::make_unique<DirectX::SpriteFont>(m_Device, upload,
					    wpath.c_str(),
					    m_FontDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
					    m_FontDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
				}
				m_Font->SetDefaultCharacter(L'?');
				DirectX::RenderTargetState rtState(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
				DirectX::SpriteBatchPipelineStateDescription spritePd(rtState);
				m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(m_Device, upload, spritePd);
				upload.End(Renderer::GetCommandQueue()).wait();
			}
			catch (...)
			{
				m_Font.reset();
				m_SpriteBatch.reset();
			}
		}

		// No mouse-hover suppression here any more - this menu is
		// keyboard-only by design, nothing in it ever reacts to the
		// cursor (see WndProcImpl below).

		// Reset the viewport/scissor rect to the full backbuffer every
		// frame - nothing else on this D3D12 device is guaranteed to
		// leave them in that state before this callback runs, and our
		// clip space (NDC) coordinates assume they map onto the whole
		// screen.
		const float width = static_cast<float>(*Pointers.ScreenResX);
		const float height = static_cast<float>(*Pointers.ScreenResY);

		D3D12_VIEWPORT viewport{0.f, 0.f, width, height, 0.f, 1.f};
		D3D12_RECT scissorRect{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
		commandList->RSSetViewports(1, &viewport);
		commandList->RSSetScissorRects(1, &scissorRect);

		// Same gate WndProcImpl already applies to input - the menu/popup
		// content stays tied to GUI::IsOpen() (the Insert toggle).
		// Notifications below are deliberately outside this gate.
		const bool menuActive = GUI::IsOpen();

		if (m_Effect && m_Batch)
		{
			m_Effect->Apply(commandList);
			m_Batch->Begin(commandList);

			if (menuActive)
			{
				g_MenuGrid.draw();
				// Same free-standing-overlay shape as MenuPopup/
				// MenuCommandBox below - see DescriptionPanel's own
				// class comment for why it needs content's own origin
				// (its own left edge/top is where the box grows left
				// from) and the sidebar's own bottom edge (whichever of
				// the two ends up lower wins, the same collision real
				// Stand's own setPositions() resolves).
				if (auto* content = MenuNavigation::Current())
				{
					const auto offsetX = static_cast<int16_t>(Theme::kMenuOriginX - Theme::kDefaultMenuOriginX);
					const auto offsetY = static_cast<int16_t>(Theme::kMenuOriginY - Theme::kDefaultMenuOriginY);
					const auto contentX = static_cast<int16_t>(content->origin.x + offsetX);
					const auto contentY = static_cast<int16_t>(content->origin.y + offsetY);

					int16_t sidebarBottomY = contentY; // no sidebar to collide with by default (Theme::kTabsVisible false)
					int16_t sidebarX = contentX;
					int16_t sidebarY, sidebarWidth, sidebarHeight;
					if (g_MenuGrid.GetSidebarRect(sidebarX, sidebarY, sidebarWidth, sidebarHeight))
					{
						(void)sidebarWidth;
						sidebarBottomY = static_cast<int16_t>(sidebarY + sidebarHeight + Theme::kSpacer);
					}
					const auto contentRightX = static_cast<int16_t>(contentX + Theme::kContentWidth + Theme::kScrollbarGap + Theme::kScrollbarWidthMutable + Theme::kSpacer);

					DescriptionPanel::Draw(contentX, contentY, contentRightX, sidebarX, sidebarBottomY);
				}
				// Drawn last, on top of everything else - see MenuPopup's
				// own class comment for why this is a free-standing
				// overlay rather than a GridItem/Grid of its own.
				MenuPopup::Draw();
				MenuCommandBox::Draw();
			}

			// Real Stand's own CommandListNotifySettings::onActiveListUpdate()
			// (Commands/Online/CommandListNotifySettings.cpp on origin/
			// stand-reference) - shows a persistent preview notification
			// live while Settings > Notifications (or its Custom Position
			// sub-page) is the one currently displayed, so every setting
			// there (Type/Position/Colour/Width/Padding) has an
			// immediately visible effect instead of needing a manual
			// "Flash Notification"/sample button press to see it. Polled
			// here every frame rather than from a Grid-level "just
			// entered/left this page" hook - Grid has no such hook yet
			// (see SettingsNotificationsGrid::IsActive()'s own comment) -
			// SetPreviewActiveImpl() itself is a no-op unless this
			// actually changes, so polling doesn't re-flash it constantly.
			Notifications::SetPreviewActive(menuActive && SettingsNotificationsGrid::IsActive());

			// Always drawn, regardless of menuActive above - see
			// Notifications.hpp's own class comment (Overlay.cpp's own
			// watermark-style FPS/business overlay, ESP, ChatDisplay,
			// Onboarding, and AutoDriveHUD are the same shape: always
			// visible, independent of any menu). MenuCommandConsole joins
			// this group rather than the menuActive-gated one above - it
			// now opens (the 'U' key) and works independently of
			// GUI::IsOpen(), same as real Stand's own Gui::showCommandBox()
			// (see its own class comment and the ungated
			// AddWindowProcedureCallback registration in Init()).
			Notifications::Draw();
			Overlay::Draw();
			ESP::Draw();
			ChatDisplay::Draw();
			Onboarding::Draw();
			AutoDriveHUD::Draw();
			MenuCommandConsole::Draw();

			m_Batch->End();
		}

		// Separate pass/batch type from the rects above: SpriteBatch manages
		// its own root signature + PSO (set in Begin()) and needs the font's
		// SRV descriptor heap bound, which replaces whatever heap the rect
		// pass above left bound.
		if (m_Font && m_SpriteBatch && m_FontDescriptorHeap)
		{
			commandList->SetDescriptorHeaps(1, m_FontDescriptorHeap.GetAddressOf());

			m_SpriteBatch->SetViewport(viewport);
			m_SpriteBatch->Begin(commandList);

			if (menuActive)
			{
				g_MenuGrid.drawText();
				if (auto* content = MenuNavigation::Current())
				{
					const auto offsetX = static_cast<int16_t>(Theme::kMenuOriginX - Theme::kDefaultMenuOriginX);
					const auto offsetY = static_cast<int16_t>(Theme::kMenuOriginY - Theme::kDefaultMenuOriginY);
					const auto contentX = static_cast<int16_t>(content->origin.x + offsetX);
					const auto contentY = static_cast<int16_t>(content->origin.y + offsetY);

					int16_t sidebarBottomY = contentY;
					int16_t sidebarX2 = contentX;
					int16_t sidebarY2, sidebarWidth2, sidebarHeight2;
					if (g_MenuGrid.GetSidebarRect(sidebarX2, sidebarY2, sidebarWidth2, sidebarHeight2))
					{
						(void)sidebarWidth2;
						sidebarBottomY = static_cast<int16_t>(sidebarY2 + sidebarHeight2 + Theme::kSpacer);
					}
					const auto contentRightX2 = static_cast<int16_t>(contentX + Theme::kContentWidth + Theme::kScrollbarGap + Theme::kScrollbarWidthMutable + Theme::kSpacer);

					DescriptionPanel::DrawText(contentX, contentY, contentRightX2, sidebarX2, sidebarBottomY);
				}
				MenuPopup::DrawText();
				MenuCommandBox::DrawText();
			}

			Notifications::DrawText();
			Overlay::DrawText();
			ESP::DrawText();
			ChatDisplay::DrawText();
			Onboarding::DrawText();
			AutoDriveHUD::DrawText();
			MenuCommandConsole::DrawText();

			m_SpriteBatch->End();
		}

		// Custom Header banner (Rendering/HeaderBanner.hpp) - drawn
		// directly above the breadcrumb header bar, spanning its own
		// width, aspect-locked to whatever's currently loaded - same
		// relationship real Stand's own GridItemHeaderAnimation has to
		// its own addressbar (see that class's own doc comment in
		// HeaderBanner.hpp). A separate SpriteBatch pass/descriptor
		// heap from the font pass above (HeaderBanner::Draw binds its
		// own), so this can run regardless of whether the embedded font
		// itself loaded.
		if (menuActive && HeaderBanner::IsLoaded())
		{
			int16_t headerX, headerY, headerWidth;
			if (g_MenuGrid.GetHeaderBarRect(headerX, headerY, headerWidth))
			{
				const auto bannerHeightH = HeaderBanner::GetRenderHeight(static_cast<float>(headerWidth));
				if (bannerHeightH > 0.f)
				{
					const auto bannerYH = static_cast<float>(headerY) - bannerHeightH - Theme::kSpacer;
					const auto posC = PosH2C(static_cast<float>(headerX), bannerYH);
					const auto sizeC = SizeH2C(static_cast<float>(headerWidth), bannerHeightH);

					HeaderBanner::Draw(commandList, viewport, posC.x, posC.y, sizeC.x, sizeC.y);
				}
			}
		}

		m_GraphicsMemory->Commit(Renderer::GetCommandQueue());
	}

	void GridRenderer::DrawRectImpl(float x, float y, float width, float height, const DirectX::XMFLOAT4& colour)
	{
		using namespace DirectX;

		// x/y/width/height are H-space (see the block comment by
		// PosH2C/SizeH2C above) - convert to real client pixels before
		// building NDC coordinates below.
		const auto posC = PosH2C(x, y);
		const auto sizeC = SizeH2C(width, height);

		VertexPositionColor v0(PixelToNdc(posC.x, posC.y), colour);
		VertexPositionColor v1(PixelToNdc(posC.x + sizeC.x, posC.y), colour);
		VertexPositionColor v2(PixelToNdc(posC.x + sizeC.x, posC.y + sizeC.y), colour);
		VertexPositionColor v3(PixelToNdc(posC.x, posC.y + sizeC.y), colour);

		m_Batch->DrawQuad(v0, v1, v2, v3);
	}

	void GridRenderer::DrawLineScreenImpl(float x1, float y1, float x2, float y2, const DirectX::XMFLOAT4& colour, float thickness)
	{
		using namespace DirectX;

		if (!m_Batch)
			return;

		// A thin quad along the line rather than a real GPU line
		// primitive - see GridRenderer.hpp's own DrawLineScreen doc
		// comment for why (reuses this same PrimitiveBatch/PSO, and
		// gives real thickness control ImGui's own AddLine had that a
		// 1px D3D12 line primitive wouldn't).
		const float dx = x2 - x1;
		const float dy = y2 - y1;
		const float length = std::sqrt(dx * dx + dy * dy);
		if (length < 0.0001f)
			return;

		const float nx = -dy / length * (thickness * 0.5f);
		const float ny = dx / length * (thickness * 0.5f);

		VertexPositionColor v0(PixelToNdc(x1 + nx, y1 + ny), colour);
		VertexPositionColor v1(PixelToNdc(x2 + nx, y2 + ny), colour);
		VertexPositionColor v2(PixelToNdc(x2 - nx, y2 - ny), colour);
		VertexPositionColor v3(PixelToNdc(x1 - nx, y1 - ny), colour);

		m_Batch->DrawQuad(v0, v1, v2, v3);
	}

	void GridRenderer::DrawRectFilledScreenImpl(float x1, float y1, float x2, float y2, const DirectX::XMFLOAT4& colour)
	{
		using namespace DirectX;

		if (!m_Batch)
			return;

		VertexPositionColor v0(PixelToNdc(x1, y1), colour);
		VertexPositionColor v1(PixelToNdc(x2, y1), colour);
		VertexPositionColor v2(PixelToNdc(x2, y2), colour);
		VertexPositionColor v3(PixelToNdc(x1, y2), colour);

		m_Batch->DrawQuad(v0, v1, v2, v3);
	}

	void GridRenderer::DrawPolygonFilledScreenImpl(const DirectX::XMFLOAT2* points, int count, const DirectX::XMFLOAT4& colour)
	{
		using namespace DirectX;

		if (!m_Batch || count < 3)
			return;

		// Fan triangulation around points[0] - only correct for a convex
		// point set, same restriction this mirrors from ImGui's own
		// AddConvexPolyFilled (see GridRenderer.hpp's own doc comment).
		const VertexPositionColor anchor(PixelToNdc(points[0].x, points[0].y), colour);
		for (int i = 1; i + 1 < count; i++)
		{
			VertexPositionColor v1(PixelToNdc(points[i].x, points[i].y), colour);
			VertexPositionColor v2(PixelToNdc(points[i + 1].x, points[i + 1].y), colour);
			m_Batch->DrawTriangle(anchor, v1, v2);
		}
	}

	void GridRenderer::DrawCircleFilledScreenImpl(float centerX, float centerY, float radius, const DirectX::XMFLOAT4& colour, int segments)
	{
		if (!m_Batch || segments < 3)
			return;

		std::vector<DirectX::XMFLOAT2> points;
		points.reserve(segments);
		for (int i = 0; i < segments; i++)
		{
			const float angle = (DirectX::XM_2PI * i) / static_cast<float>(segments);
			points.push_back({centerX + std::cos(angle) * radius, centerY + std::sin(angle) * radius});
		}

		DrawPolygonFilledScreenImpl(points.data(), static_cast<int>(points.size()), colour);
	}

	void GridRenderer::DrawTextImpl(float x, float y, const char* text, const DirectX::XMFLOAT4& colour, float scale)
	{
		if (!m_Font || !m_SpriteBatch)
			return;

		const auto posC = PosH2C(x + Theme::kTextOffsetX, y + Theme::kTextOffsetY);
		const auto finalScale = scale * GetResolutionTextScale(GetClientSize());

		m_Font->DrawString(m_SpriteBatch.get(), text, DirectX::XMFLOAT2(posC.x, posC.y), DirectX::XMLoadFloat4(&colour), 0.f, DirectX::XMFLOAT2{0.f, 0.f}, DirectX::XMFLOAT2{finalScale, finalScale});
	}

	void GridRenderer::DrawTextScreenImpl(float x, float y, const char* text, const DirectX::XMFLOAT4& colour, float scale)
	{
		if (!m_Font || !m_SpriteBatch)
			return;

		// Real client pixels, no PosH2C/resolution-scale correction -
		// see GridRenderer.hpp's own DrawTextScreen doc comment for why.
		m_Font->DrawString(m_SpriteBatch.get(), text, DirectX::XMFLOAT2(x, y), DirectX::XMLoadFloat4(&colour), 0.f, DirectX::XMFLOAT2{0.f, 0.f}, DirectX::XMFLOAT2{scale, scale});
	}

	DirectX::XMFLOAT2 GridRenderer::MeasureTextImpl(const char* text, float scale) const
	{
		if (!m_Font)
			return {};

		DirectX::XMFLOAT2 size;
		DirectX::XMStoreFloat2(&size, m_Font->MeasureString(text));
		size.x *= scale;
		size.y *= scale;
		return size;
	}

	void GridRenderer::WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (!GUI::IsOpen())
			return;

		// MenuCommandBox takes over every input while open, same
		// precedence MenuPopup already has (see its own comment right
		// below) and checked first for the same reason - nothing in
		// this system ever opens both at once, but a command box open
		// on top of a focused text field mid-edit should still win.
		// Unlike MenuPopup it also needs WM_CHAR (typed digits/letters),
		// not just WM_KEYDOWN (Enter/Escape/Backspace) - see MenuCommandBox::
		// HandleChar()/HandleKey()'s own doc comments for the split.
		if (MenuCommandBox::IsOpen())
		{
			if (msg == WM_CHAR)
				MenuCommandBox::HandleChar(static_cast<wchar_t>(wparam));
			else if (msg == WM_KEYDOWN)
				MenuCommandBox::HandleKey(static_cast<unsigned int>(wparam));

			return;
		}

		// MenuCommandConsole is NOT handled here (unlike MenuCommandBox
		// right above) - it has its own separate, ungated
		// AddWindowProcedureCallback registration (see Init()) so both
		// opening it (the 'U' key) and routing input to it while open
		// work regardless of GUI::IsOpen(), matching real Stand's own
		// Gui::showCommandBox() (no g_gui.opened check there either -
		// confirmed against origin/stand-reference). Handling it in both
		// places would double-fire every keystroke while the menu
		// happens to also be open.

		// MenuPopup takes over every input while open, ahead of even the
		// text-edit interception right below - a popup open on top of a
		// text field mid-edit still wins. See MenuPopup's own class
		// comment for why it always swallows regardless of what it
		// matched. Keyboard-only, same as everything else here - the
		// unconditional return below swallows mouse messages too, same
		// as any other message type this doesn't explicitly act on.
		if (MenuPopup::IsOpen())
		{
			if (msg == WM_KEYDOWN)
				MenuPopup::HandleKey(static_cast<unsigned int>(wparam));

			return;
		}

		// Text-edit interception: while the keyboard-focused item is a
		// GridItemTextInput (or anything built on it) actively editing -
		// see GridItem.hpp's own isEditingText()/onChar()/onEditKey() doc
		// comments - every character/control key goes to it instead of
		// MenuGrid::HandleKey()'s normal list navigation, the same way
		// ImGui's own WantCaptureKeyboard steals input from the game
		// while an ImGui text field has focus. Checked before anything
		// else below since typing shouldn't ever fall through to
		// something else.
		if (auto* focused = MenuFocus::GetFocusedItem(MenuNavigation::Current()); focused && focused->isEditingText())
		{
			if (msg == WM_CHAR)
			{
				focused->onChar(static_cast<wchar_t>(wparam));
				return;
			}

			if (msg == WM_KEYDOWN)
			{
				focused->onEditKey(static_cast<unsigned int>(wparam));
				return;
			}
		}

		// Mouse is deliberately NOT wired to the menu at all - reported
		// bug: with WM_MOUSEMOVE/WM_LBUTTONDOWN/WM_MOUSEWHEEL routed into
		// MenuGrid (a prior pass here added this as "real mouse support,
		// same as real Stand has"), the mouse wheel scrolled the content
		// list any time the cursor happened to sit over it - completely
		// unintended while just driving/aiming, not trying to touch the
		// menu at all. Checked what real Stand actually gates its own
		// input-blocking on rather than guessing: origin/stand-reference's
		// own src/AntiCheat/NativeTableHooksBuiltin.cpp only ever
		// suppresses game input while its Commandbox::shouldBlockGameInputs()
		// is true (i.e. while its free-text command box is actively
		// capturing keystrokes) - never merely because its menu is open
		// and being browsed. This project's own MenuGrid::HandleKey()
		// only ever acts on Up/Down/Left/Right/Enter/Backspace/Ctrl/
		// Shift (see its own switch statement) - driving/aiming/shooting
		// keys are never touched by it - so keyboard-only navigation
		// here doesn't cost anything mouse support was providing beyond
		// itself; onClick()/onClickEx()/onArrow() are all still there on
		// every widget for a future pass that wants to re-add real mouse
		// support with a fix for the wheel-hijack above, just not called
		// from here any more. See RawInput.cpp's own comment for the
		// other half of this bug (mouse clicks not reaching the game at
		// all while the menu was open).

		// Every other key this system responds to (Up/Down/Left/Right/
		// Enter/Backspace - see MenuGrid::HandleKey()) - the InputCapture
		// check here is largely defensive at this point (the branch
		// above already returns for WM_CHAR/WM_KEYDOWN while a GridItem
		// text field is focused and editing), but keeps this from firing
		// on the same frame such a field starts/stops editing. WM_KEYDOWN
		// repeats while a key is held (standard Win32 auto-repeat), which
		// is exactly the "hold Down to keep moving" feel a menu like this
		// should have - nothing extra needed here for that.
		if (msg == WM_KEYDOWN && !Rendering::InputCapture::IsTextInputActive())
			g_MenuGrid.HandleKey(static_cast<unsigned int>(wparam));
	}

	void GridRenderer::Init()
	{
		// Custom Header's local image folder - created here (unlike
		// CommandTabsPos's Sync(), which only ever runs from LoadState()/
		// OnChange() and so never fires on a fresh config with no saved
		// "header" entry yet - see Commands::LoadStateImpl) so the
		// folder exists and is discoverable as soon as the menu is
		// ready, whether or not the user has ever touched Settings >
		// Appearance > Position yet. Safe to call this early - FileMgr::
		// Init() has already run by the time Main() reaches this call
		// (see main.cpp).
		FileMgr::GetProjectFolder("./Headers");

		Renderer::AddDirect3DDrawCallBack(
		    [](ID3D12GraphicsCommandList* commandList) {
			    GetInstance().DrawImpl(commandList);
		    },
		    0);

		Renderer::AddWindowProcedureCallback([](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
			GetInstance().WndProcImpl(hwnd, msg, wparam, lparam);
		});

		// Deliberately separate from WndProcImpl above (and ungated by its
		// own GUI::IsOpen() check) - see Onboarding.hpp's own class
		// comment for why: it has to be interactable even before the menu
		// has ever been opened.
		Renderer::AddWindowProcedureCallback([](HWND, UINT msg, WPARAM wparam, LPARAM) {
			if (msg == WM_KEYDOWN)
				Onboarding::HandleKey(static_cast<unsigned int>(wparam));
		});

		// Same reason as Onboarding's own callback right above -
		// deliberately ungated by GUI::IsOpen() so MenuCommandConsole
		// opens (the 'U' key) and works whether or not the main menu is
		// open, matching real Stand's own Gui::showCommandBox() (no
		// g_gui.opened check there either - confirmed against
		// origin/stand-reference). Handles both routing input to it
		// while already open and the 'U' key that opens it - see
		// WndProcImpl's own comment for why none of this lives there
		// instead (it would double-fire every keystroke while the menu
		// also happens to be open).
		Renderer::AddWindowProcedureCallback([](HWND, UINT msg, WPARAM wparam, LPARAM) {
			if (MenuCommandConsole::IsOpen())
			{
				if (msg == WM_CHAR)
					MenuCommandConsole::HandleChar(static_cast<wchar_t>(wparam));
				else if (msg == WM_KEYDOWN)
					MenuCommandConsole::HandleKey(static_cast<unsigned int>(wparam));
			}
			else if (msg == WM_KEYDOWN && wparam == 'U' && !Rendering::InputCapture::IsTextInputActive())
			{
				MenuCommandConsole::Open();
			}
		});
	}
}
