#pragma once
#include "Rendering/StandPort/Position2d.hpp"
#include "Rendering/StandPort/TextSettings.hpp"

#include <DirectXMath.h>

#include <atomic>
#include <cstdint>
#include <string>

namespace Stand::Rendering::Theme
{
	inline DirectX::XMFLOAT4 kAccent{1.f, 0.f, 1.f, 1.f};
	inline std::atomic<uint32_t> kNavBarColour{0u};
	inline DirectX::XMFLOAT4 kPanelBackground{0.f, 0.f, 0.f, 0.3019f};
	inline DirectX::XMFLOAT4 kText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kFocusText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kFocusRightText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kFocusTexture{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kUnfocusedText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kUnfocusedRightText{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kUnfocusedTexture{1.f, 1.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kHud{1.f, 0.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kAr{1.f, 0.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kMinigame{1.f, 0.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kToggleOff{0.35f, 0.35f, 0.35f, 1.f};
	inline DirectX::XMFLOAT4 kError{0.6f, 0.2f, 0.2f, 1.f};
	inline DirectX::XMFLOAT4 kPlaceholderText{0.7f, 0.7f, 0.7f, 1.f};

	void LoadFromDisk();
	void SaveToDisk();

	constexpr int16_t kHeaderHeight = 24;
	constexpr int16_t kSidebarWidth = 112;
	constexpr int16_t kSidebarEntryHeight = 32;
	constexpr int16_t kContentWidth = 450;
	constexpr int16_t kContentItemHeight = 32;

	inline int16_t kInfoWidth = 300;
	enum class InfoTextPosition : uint8_t { Left, Bottom, Right, BelowTabs };
	inline InfoTextPosition kInfoTextPosition = InfoTextPosition::Left;
	inline int16_t kInfoPadding = 0;
	inline bool kShowHelpText = true;
	inline bool kShowSyntax = true;
	inline bool kShowSliderBehaviour = true;
	inline bool kShowNonUserOnly = true;
	inline bool kShowOwners = true;

	constexpr float kTextScale = 0.65625f;
	constexpr float kSmallTextScale = kTextScale * (12.f / 15.f);

	constexpr float kTextOffsetX = -2.f;
	constexpr float kTextOffsetY = 2.f;
	constexpr float kHudWidth = 1920.f;
	constexpr float kHudHeight = 1080.f;

	constexpr int16_t kContentBottomMargin = 20;
	constexpr int16_t kScrollbarWidth = 4;
	constexpr int16_t kScrollbarGap = 4;
	constexpr int16_t kSpacer = 3;
	constexpr int16_t kDefaultMenuOriginX = 1323;
	constexpr int16_t kDefaultMenuOriginY = 560;
	inline int16_t kMenuOriginX = kDefaultMenuOriginX;
	inline int16_t kMenuOriginY = kDefaultMenuOriginY;
	inline bool kAddressBarVisible = true;
	inline int16_t kAddressBarHeight = kHeaderHeight;
	inline std::string kAddressBarRootName = "";
	inline std::string kAddressSeparator = " > ";
	inline bool kAddressCurrentListOnly = false;
	inline bool kAddressbarWidthAffectedByScrollbar = false;
	inline bool kAddressbarWidthAffectedByColumns = false;
	inline TextSettings kAddressbarText{ 0.525f, -2.f, 2.f };
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
	inline bool kDisableEntityPreviews = false;
	inline bool kPreviewOpaque = false;
	inline float kPreviewRotationSpeed = 1.f;
	inline float kPreviewPedDist = 0.f;
	inline float kPreviewObjDist = 0.f;
	inline float kPreviewVehDist = 0.f;

	enum class LeftTexturesMode : uint8_t { Disabled, AllCommands, AllCommandsCompact, TogglesOnly, TogglesOnlyCompact };
	inline LeftTexturesMode kLeftTextures = LeftTexturesMode::Disabled;

	inline std::string kFontPath = "";
	inline bool kFontReloadPending = false;

	inline int8_t kColumns = 1;
	inline uint8_t kMenuHeight = 11; // Stand's own command_rows default
	inline int16_t kListWidth = 0;
	inline int16_t kListHeight = 0;
	inline int16_t kSpacerSize = 0;
	inline int16_t kSmoothScroll = 0;
	inline uint8_t kBlur = 0;

	enum class InputPreset : uint8_t { Flexible, NoNumpad, Numpad };
	inline InputPreset kInputPreset = InputPreset::Flexible;

	inline bool kMouse = false;
	inline bool kMouseCursorOnNavigation = true;
	inline bool kControllerSupport = false;
	inline bool kBackReset = false;
	inline bool kBackClose = false;
	inline bool kInterruptHoldWrapAround = true;
	inline bool kTabsKeepCursor = true;
	inline bool kListsKeepCursor = true;
	inline bool kListsKeepCursorReducedHuge = true;
	inline int kKeyRepeatInterval = 0;
	inline int kKeyLongRepeatInterval = 0;
	inline int kTabRepeatInterval = 0;
	inline bool kDisableHotkeys = false;

	enum class ModifierKeyMatching : uint8_t { Strict, Smart, Lax };
	inline ModifierKeyMatching kModifierKeyMatching = ModifierKeyMatching::Smart;

	inline uint8_t kControllerOpenClose1 = 0;
	inline uint8_t kControllerOpenClose2 = 0;
	inline uint8_t kControllerPrevTab = 0;
	inline uint8_t kControllerNextTab = 0;
	inline uint8_t kControllerUp = 0;
	inline uint8_t kControllerDown = 0;
	inline uint8_t kControllerLeft = 0;
	inline uint8_t kControllerRight = 0;
	inline uint8_t kControllerClick = 0;
	inline uint8_t kControllerBack = 0;
	inline uint8_t kControllerContextMenu = 0;
	inline uint8_t kControllerCommandBox = 0;

	enum class TabsPosition : uint8_t
	{
		Left,
		Right,
		Top,
		Bottom,
	};
	inline TabsPosition kTabsPosition = TabsPosition::Left;
	inline bool kTabsVisible = true;

	[[nodiscard]] Position2d GetContentOrigin();
	[[nodiscard]] Position2d GetTabbedContentOrigin();
}
