#pragma once
#include "Rendering/Position2d.hpp"

#include <DirectXMath.h>

#include <cstdint>
#include <string>

namespace Stand::Rendering::Theme
{
	// Stand's own default (unthemed) look - genuinely verified this time
	// against origin/stand-reference (a branch of this same repo, not
	// something fetchable from the open internet - a previous pass here
	// wrongly assumed it didn't exist and edited several comments to hedge
	// that; that hedge was itself wrong). See that branch's own src/
	// Rendering/Renderer.hpp, the Renderer class's own default member
	// values: focusRectColour = {1,0,1,1}, bgRectColour = {0,0,0,0.3019},
	// bgTextColour = {1,1,1,1}, addressbar_height = 24, tabs_width = 112,
	// tabs_height = 32, command_width = 450, command_height = 32 - every
	// one matches what this file already had.

	// The one accent colour: whatever's currently active, on, selected,
	// or otherwise the thing that matters right now - a sidebar/tab's
	// active entry, a toggle's ON state, a button's fill. Stand's own
	// focusRectColour.
	//
	// This and the five colours below are `inline` (mutable), not
	// `constexpr`, specifically so Settings > Customize (CustomizeGrid)
	// can edit them at runtime - see its own class comment for why this
	// project's actual theme editor lives there instead of the classic
	// menu's own ImGui-style Customize page (which edited ImGuiStyle/
	// ImGuiCol_*, meaningless once this Grid renderer is primary). Every
	// existing `Theme::kAccent`-style reference elsewhere in this
	// codebase keeps working unchanged either way - dropping `constexpr`
	// doesn't change how the name is used at any call site, only that it
	// can now change after startup. LoadFromDisk()/SaveToDisk() persist
	// these six; nothing else in this file is user-editable (layout/
	// scale constants stay `constexpr`).
	inline DirectX::XMFLOAT4 kAccent{1.f, 0.f, 1.f, 1.f};

	// The translucent panel background every non-active row uses -
	// toggle/folder rows, inactive sidebar/tab entries, a stepper/list's
	// value box. Stand's own bgRectColour, confirmed 0.3019 alpha exactly
	// (see this file's own top comment).
	//
	// A previous pass here invented a second, more-opaque "backdrop"
	// constant on top of this one, on the theory that real Stand must be
	// compositing some separate solid panel underneath its rows - it
	// isn't. Checked against origin/stand-reference's own src/Menu/
	// GridItemList.cpp: real Stand draws bgRectColour exactly once, as a
	// single rect spanning the whole (unfocused) list - not per row - and
	// its rows have zero gap between them (no separate spacer; they're
	// just offsets inside one GridItemList, not individually positioned
	// GridItems). The reason this project's own rows read as "very
	// transparent" was never that 0.3019 was too low - it's that every
	// row here IS its own separate top-level GridItem, positioned through
	// Grid::setPositions() using this content Grid's own spacer_size,
	// which every content Grid's own constructor set to 3 - the same gap
	// real Stand only ever uses between distinct chrome pieces
	// (addressbar/tabs/list), never between individual rows within one.
	// That left a real, unrendered 3-unit seam of bare game world between
	// every single row. Fixed at the source instead: every content Grid's
	// own spacer_size is 0 now (see each one's own constructor comment),
	// so rows pack edge to edge the same way Stand's own list does, and
	// bgRectColour's existing 30% alpha reads as one continuous panel
	// rather than a stack of separately-transparent strips.
	inline DirectX::XMFLOAT4 kPanelBackground{0.f, 0.f, 0.f, 0.3019f};

	// White text/foreground, used everywhere. Stand's own
	// bgTextColour/focusTextColour (identical in both states there).
	inline DirectX::XMFLOAT4 kText{1.f, 1.f, 1.f, 1.f};

	// Focused-row colour variants — Stand's own focusTextColour,
	// focusRightTextColour, focusSpriteColour. Default white matches real
	// Stand where bgTextColour == focusTextColour == {1,1,1,1}.
	inline DirectX::XMFLOAT4 kFocusText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kFocusRightText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kFocusTexture{1.f, 1.f, 1.f, 1.f};

	// Unfocused-row colour variants — Stand's own bgTextColour,
	// bgRightTextColour, bgSpriteColour (commands: unfocusedtext,
	// unfocusedrighttext, unfocusedtexture). Default white same as kText.
	inline DirectX::XMFLOAT4 kUnfocusedText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kUnfocusedRightText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kUnfocusedTexture{1.f, 1.f, 1.f, 1.f};

	// Renderer overlay colours for HUD, AR, and Minigame elements —
	// Stand's own g_renderer.hudColour/arColour/minigameColour. Default
	// pink matches real Stand's own primaryColour copy-from default.
	inline DirectX::XMFLOAT4 kHud{1.f, 0.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kAr{1.f, 0.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kMinigame{1.f, 0.f, 1.f, 1.f};

	// NOT part of Stand's own palette - its Renderer only exposes one
	// accent (focusRectColour) and one panel colour (bgRectColour), with
	// no dedicated "off"/"unknown" look of its own. These are this
	// project's own additions for states that palette has nothing to
	// say about: a toggle's OFF state, a command hash that didn't
	// resolve to anything registered, this system's own "not yet
	// migrated" placeholder text.
	inline DirectX::XMFLOAT4 kToggleOff{0.35f, 0.35f, 0.35f, 1.f};
	inline DirectX::XMFLOAT4 kError{0.6f, 0.2f, 0.2f, 1.f};
	inline DirectX::XMFLOAT4 kPlaceholderText{0.7f, 0.7f, 0.7f, 1.f};

	// Loads the six colours above from disk (a no-op, keeping the
	// defaults above, if nothing's been saved yet) - called once, lazily,
	// from CustomizeGrid's own populate() (not its constructor - see
	// GlobalsGrid.cpp's identical note on why a file-scope static Grid's
	// constructor runs too early, before FileMgr::Init(), to safely
	// touch disk). Safe to call more than once; only the first call
	// actually reads anything.
	void LoadFromDisk();

	// Saves the six colours above to disk - called after every edit
	// CustomizeGrid makes, same as the classic Customize page's own
	// GUISettings.cpp calling SaveSettings() after each change.
	void SaveToDisk();

	// Layout - int16_t, same as Stand's own addressbar_height/tabs_width/
	// tabs_height/command_width/command_height fields (and GridItem's
	// own x/y/width/height, ported the same way - see GridItem.hpp).
	// addressbar_height is the header/breadcrumb bar; tabs_width/
	// tabs_height are the left-hand submenu list ("sidebar" everywhere
	// else in this system); command_width/command_height are the main
	// content list's own column width and each row's height. Confirmed
	// exact against origin/stand-reference's own Renderer.hpp (see this
	// file's own top comment) - a previous pass here shrank
	// kSidebarEntryHeight/kContentItemHeight to 26 on the mistaken belief
	// that no real source existed to check them against; reverted back
	// to the confirmed-correct 32. The actual, source-backed reason
	// real Stand's rows still look tighter is the zero-gap fix on every
	// content Grid's own spacer_size below, not a shorter row.
	constexpr int16_t kHeaderHeight = 24;
	constexpr int16_t kSidebarWidth = 112;
	constexpr int16_t kSidebarEntryHeight = 32;
	constexpr int16_t kContentWidth = 450;
	constexpr int16_t kContentItemHeight = 32;

	// Stand's own MenuGrid::info_width default (Menu/MenuGrid.hpp) - the
	// fixed width of the focused command's own description box under
	// its DEFAULT "Left" position (CommandHelpPos's own ALIGN_TOP_LEFT,
	// confirmed against origin/stand-reference's own MenuGrid.cpp
	// populateBody(): info_text_width is only ever command_width for the
	// ALIGN_BOTTOM_LEFT case specifically - every other position,
	// "Left" included, uses this instead). See DescriptionPanel's own
	// class comment for the placement this project ports (Left, not
	// Below Tabs - a previous pass here had this backwards).
	inline int16_t kInfoWidth = 300;
	enum class InfoTextPosition : uint8_t { Left, Bottom, Right, BelowTabs };
	inline InfoTextPosition kInfoTextPosition = InfoTextPosition::Left;
	inline int16_t kInfoPadding = 0;
	inline bool kShowHelpText = true;
	inline bool kShowSyntax = true;
	inline bool kShowSliderBehaviour = true;
	inline bool kShowNonUserOnly = true;
	inline bool kShowOwners = true;

	// Stand's own command_text/tabs_text scale: float(15.0 *
	// TEXT_HEIGHT_PX * 2.0), TEXT_HEIGHT_PX = 0.021875 - both confirmed
	// exact against origin/stand-reference's src/Rendering/dx_common.hpp
	// and the TextSettings members in Renderer.hpp. Stand's embedded
	// SpriteFont (the same "Be Vietnam Pro" blob this project embeds -
	// see font_bevietnamprolight.hpp) is baked much larger than any row
	// here is meant to show it at, so every text draw needs this scale
	// applied - drawing it at the font's native size (scale 1.0, what
	// this project did before) is why text looked comically oversized
	// for its row. GridRenderer applies this once, uniformly, in
	// DrawText()/MeasureText() rather than per-widget.
	//
	// NOT the same scale addressbar_text uses in real Stand, though -
	// its own TextSettings is 12.0-based (Menu/MenuGrid.hpp), not 15.0
	// like tabs_text/command_text. This project applies kTextScale to
	// header text too (no separate scale plumbed through yet), a real,
	// disclosed gap from real Stand rather than a verified match - left
	// alone here since fixing it means threading a second scale through
	// GridRenderer's text path, out of scope for this pass.
	constexpr float kTextScale = 0.65625f;
	constexpr float kSmallTextScale = kTextScale * (12.f / 15.f);

	constexpr float kTextOffsetX = -2.f;
	constexpr float kTextOffsetY = 2.f;

	// Stand's own virtual HUD canvas dimensions (see GridRenderer::
	// PosH2C/SizeH2C's own doc comment for what "H-space" means) - shared
	// here since both GridRenderer.cpp (H-space <-> client-pixel
	// conversion) and MenuGrid.cpp (working out how much of a content
	// Grid's own list is actually visible, for scrolling - see
	// Grid::ScrollBy()/ScrollToShow()) need the same numbers.
	constexpr float kHudWidth = 1920.f;
	constexpr float kHudHeight = 1080.f;

	// Bottom margin (H-space) a content Grid's own visible scroll window
	// stops short of the HUD canvas's bottom edge by - same idea as
	// kHeaderX/kHeaderY's own 20px margins elsewhere in this system, just
	// for the opposite edge.
	constexpr int16_t kContentBottomMargin = 20;

	constexpr int16_t kScrollbarWidth = 4;
	constexpr int16_t kScrollbarGap = 4;

	// Same spacer_size every content Grid's own constructor and
	// MenuGrid.cpp's own header/sidebar stacking use - pulled out here
	// (rather than each restating the literal 3) since GetContentOrigin()
	// below and MenuGrid.cpp's own layout both need the exact same
	// number to agree on where the sidebar/tab-strip gap actually is.
	constexpr int16_t kSpacer = 3;

	// Real Stand's own MenuGrid::default_origin ({1323, 560} - see
	// MenuGrid.cpp's own kHeaderX/kHeaderY, which reference these instead
	// of restating the same numbers) - the position every content Grid's
	// own hardcoded (1438, 587)-style origin already assumes. Settings >
	// Appearance > Position (menux/menuy) edits kMenuOriginX/Y at
	// runtime; kDefaultMenuOriginX/Y stay fixed so Grid::forEachVisibleItem()
	// (Grid.cpp) can work out how far the menu has been moved from where
	// every content Grid's own origin was written assuming it'd sit -
	// see that function's own comment for why the offset is applied
	// there rather than baked into GridRenderer's own PosH2C (which
	// every non-menu overlay - Notifications, MenuPopup, MenuCommandBox,
	// Onboarding - also draws through and must never shift just because
	// the menu did).
	constexpr int16_t kDefaultMenuOriginX = 1323;
	constexpr int16_t kDefaultMenuOriginY = 560;
	inline int16_t kMenuOriginX = kDefaultMenuOriginX;
	inline int16_t kMenuOriginY = kDefaultMenuOriginY;

	inline bool kAddressBarVisible = true;
	inline int16_t kAddressBarHeight = kHeaderHeight;
	inline std::string kAddressSeparator = " > ";
	inline bool kAddressCurrentListOnly = false;
	inline bool kAddressbarWidthAffectedByScrollbar = false;
	inline bool kAddressbarWidthAffectedByColumns = false;
	inline float kAddressbarTextScale = kSmallTextScale;
	inline float kAddressbarTextXOffset = 5.f;
	inline float kAddressbarTextYOffset = 0.f;
	inline bool kShowCursorPos = false;
	inline bool kCursorPosIncludesDividers = false;

	inline int kCursorPadding = 2;
	inline int16_t kCursorBorderWidth = 0;
	inline bool kCursorBorderRounded = false;
	inline DirectX::XMFLOAT4 kCursorBorderColour{1.f, 1.f, 1.f, 1.f};

	inline int16_t kBorderWidth = 0;
	inline bool kBorderRounded = false;
	inline DirectX::XMFLOAT4 kBorderColour{1.f, 1.f, 1.f, 1.f};

	inline int16_t kTabsWidth = kSidebarWidth;
	inline int16_t kTabsHeight = kSidebarEntryHeight;
	inline float kTabsTextScale = kTextScale;
	inline float kTabsTextXOffset = 5.f;
	inline float kTabsTextYOffset = 0.f;
	enum class TabsAlignment : uint8_t { Left, Centre, Right };
	inline TabsAlignment kTabsAlignment = TabsAlignment::Left;
	inline bool kTabsShowLeftIcon = false;
	inline bool kTabsShowName = true;
	inline bool kTabsShowRightIcon = false;
	inline bool kTabsIconsUseTextColour = false;

	enum class ScrollbarMode : uint8_t { Enabled, EnabledWhenNeeded, Disabled };
	inline ScrollbarMode kScrollbarMode = ScrollbarMode::EnabledWhenNeeded;
	inline int16_t kScrollbarWidthMutable = 6;

	inline float kCommandTextScale = kTextScale;
	inline float kCommandTextXOffset = 5.f;
	inline float kCommandTextYOffset = 0.f;
	inline bool kPreviewColourInTexture = true;
	inline bool kPreviewColourInList = true;
	inline bool kPrefillSliders = true;
	inline bool kPrefillTextInputs = true;
	inline bool kSelectableNASliders = false;
	inline bool kSlidersRightbound = false;
	inline bool kSelectableDividers = false;

	inline float kCommandBoxInputScale = kTextScale;
	inline float kCommandBoxInputXOffset = 0.f;
	inline float kCommandBoxInputYOffset = 0.f;

	inline float kSmallTextScaleMutable = kSmallTextScale;
	inline float kSmallTextXOffset = 0.f;
	inline float kSmallTextYOffset = 0.f;

	inline bool kShowTextBoundingBoxes = false;

	inline std::string kFontPath = "";
	inline bool kFontReloadPending = false;

	inline int8_t kColumns = 1;
	inline uint8_t kMenuHeight = 20;
	inline int16_t kListWidth = 0;
	inline int16_t kListHeight = 0;
	inline int16_t kSpacerSize = 0;
	inline int16_t kSmoothScroll = 0;
	inline uint8_t kBlur = 0;

	// Real Stand's own Direction enum (Util/Direction.hpp on
	// origin/stand-reference), narrowed to just the four values its own
	// g_renderer.tabs_pos actually uses for the sidebar/tab strip -
	// Settings > Appearance > Tabs Position (CommandTabsPos) edits
	// kTabsPosition at runtime, same "inline mutable global" pattern as
	// kMenuOriginX/Y above. kTabsVisible is the same pattern for real
	// Stand's own CommandTabs (show/hide the whole sidebar).
	enum class TabsPosition : uint8_t
	{
		Left,
		Right,
		Top,
		Bottom,
	};
	inline TabsPosition kTabsPosition = TabsPosition::Left;
	inline bool kTabsVisible = true;

	// The origin every content Grid's own constructor uses, for the
	// current kTabsPosition/kTabsVisible - (1438, 587) for the still-
	// default Left+visible case, confirmed exactly against
	// kDefaultMenuOriginX/Y + kSidebarWidth/kHeaderHeight/kSpacer above
	// (see Theme.cpp's own definition) so nothing shifts for a user who
	// never touches Settings > Appearance > Tabs Position. MenuGrid.cpp's
	// own header/sidebar construction uses the same numbers to lay out
	// its own chrome around whatever this returns - see that file for
	// how the two stay in sync.
	//
	// Evaluated once, at each content Grid's own construction (every
	// content Grid is a file-static/global instance built long before
	// Settings > Appearance ever runs) - same limitation Grid.hpp's own
	// class comment already discloses for this project's populate()
	// model (no Worker-driven live-repopulation yet): changing Tabs
	// Position at runtime moves MenuGrid's own header/sidebar chrome
	// immediately (see MenuGrid::populate(), rebuilt via invalidate()),
	// but every already-constructed content Grid's own origin was fixed
	// at startup and does not retroactively follow a later change - a
	// real, disclosed gap rather than a silent one, left alone here since
	// closing it means giving every content Grid a way to be found and
	// re-originated live, a whole registry this project doesn't have yet.
	[[nodiscard]] Position2d GetContentOrigin();

	// Same as GetContentOrigin() above, offset further down for a
	// GridTabbed page's own tab-strip children (VehicleSpawnGrid's
	// New/Personal Vehicle, RecoveryHeistsGrid's six heist types,
	// ScriptsGrid's Threads/Start Script) - (1438, 622) for the
	// still-default case, the exact same "content origin + one
	// kContentItemHeight-tall strip + kSpacer" arithmetic those pages'
	// own child Grid constructors already hardcoded.
	[[nodiscard]] Position2d GetTabbedContentOrigin();
}
