#pragma once
#include <DirectXMath.h>

#include <cstdint>

namespace Stand::Rendering::NotifySettings
{
	// Settings > Notifications - real Stand's own CommandListNotifySettings
	// (Commands/Online/CommandListNotifySettings.cpp on origin/stand-reference).
	// Mutable globals, same shape as Theme.hpp's own six colours - read by
	// Notifications.cpp's own draw path, written by the Commands in
	// src/Commands/Settings/CommandNotify*.cpp. Kept in their own header
	// (not folded into Theme.hpp or Notifications.hpp) so the Commands
	// don't need to pull in Notifications.hpp's own FiberPool/mutex/
	// Notification-struct machinery just to reach these.

	// Real Stand's own CommandNotifyType - which of its three notification
	// styles is active. Game routes straight through Notifications::
	// ShowInGame() (the native GTA5 feed, already in this codebase) instead
	// of drawing this project's own DirectXTK12 toast overlay at all.
	enum class Type
	{
		Game,
		StandNextToMap,
		StandCustomPosition,
	};

	inline Type kType = Type::StandNextToMap;

	// Real Stand's own Invert Flow - stacks newer notifications upward
	// from the anchor point instead of downward when true.
	inline bool kInvertFlow = false;

	// Real Stand's own Width/Padding defaults (400/10) - Width replaces
	// this project's own previous hardcoded card width (350, an arbitrary
	// leftover from the pre-port ImGui window size, not anything real
	// Stand ever used); Padding is the gap between the anchor point/
	// stacked cards, this project's own previous hardcoded kMargin (10 -
	// happens to already match Stand's own default exactly).
	inline float kWidth = 400.f;
	inline float kPadding = 10.f;

	// Real Stand's own "next to map" position (NotifyGrid::
	// setOriginNextToMap(), origin/stand-reference's src/Menu/
	// NotifyGrid.cpp) has no fixed value at all - it's computed live
	// every time the safe zone size or minimap visibility changes, from
	// GRAPHICS::GET_SAFE_ZONE_SIZE() and a few HUD-state natives, then
	// shifted right if the minimap is actually on screen so this sits
	// BESIDE it rather than overlapping it. See Notifications.cpp's own
	// GetNextToMapAnchor() for this project's port of that formula -
	// kept local there (not a setting here) since there's nothing for a
	// Command to bind to; it's derived state, not a user preference.

	// Real Stand's own CommandNotifyPosition default ({32, 532}) - only
	// takes effect while kType == StandCustomPosition.
	inline int16_t kCustomPositionX = 32;
	inline int16_t kCustomPositionY = 532;

	// Real Stand's own CommandNotifyBorderColour/FlashColour/BgColour
	// defaults - Border/Background both default to a copy of Primary/
	// Background Colour respectively (matching Theme::kAccent/
	// kPanelBackground's own current values, same numbers real Stand's
	// own defaults happen to already be); Flash Colour has its own
	// distinct default (a magenta/purple, unrelated to any other colour).
	inline DirectX::XMFLOAT4 kBorderColour{1.f, 0.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kFlashColour{0.6196f, 0.f, 0.6196f, 1.f};
	inline DirectX::XMFLOAT4 kBackgroundColour{0.f, 0.f, 0.f, 77.f / 255.f};

	// Real Stand's own GridToaster defaults (Menu/GridToaster.hpp) - a
	// notification's own display duration is computed from these (word
	// count / reading speed, plus a start delay, clamped to
	// [kMinDurationMs, kMaxDurationMs]) rather than a duration passed in
	// per call - see Notifications.cpp's own EstimateReadingTime().
	inline std::uint16_t kReadingSpeedWpm = 250;
	inline int kReadingStartDelayMs = 500;
	inline int kMinDurationMs = 2000;
	inline int kMaxDurationMs = 10000;

	// Real Stand's own FLASH_MS (Menu/GridToaster.cpp) - how long a
	// notification's border shows Flash Colour instead of Border Colour
	// after it first appears (or re-triggers - see Notifications.cpp's
	// own ShowImpl()).
	constexpr int kFlashMs = 500;
}
