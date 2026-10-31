# Stand GUI Port Checklist

Track which Stand GUI systems are ported, partial, or still needed for 1:1 parity.

**Legend:** ✅ Done | 🔶 Partial | ❌ To Do | ➖ N/A (DX11 only / cannot port as-is)

---

## Visual Rendering

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | Theme Icons — 76 slots, SpriteBatch, embedded PNG fallbacks, Custom→Uni fallback | `Rendering/StandPort/ThemeIcons.hpp/cpp`, `ThemeIconsData.hpp` | `Renderer.hpp` |
| ✅ | Icon left-column spacing (`left_space_before_all_commands`) | `Rendering/MenuGrid.hpp` | `menu_grid.left_space_before_all_commands` |
| ✅ | Right-aligned slider values (`< value >`) | `Rendering/GridItemList.cpp` | `GridItemList.hpp/cpp` |
| ✅ | Focused-item accent highlight rect | `Rendering/GridItemList.cpp` | `GridItemList.cpp` |
| ✅ | Description panel below list | `Rendering/DescriptionPanel.hpp/cpp` | `DescriptionPanel.hpp/cpp` |
| ✅ | Icon leftbound/rightbound mode (`leftbound_textures_toggles` / `leftbound_textures_nontoggles`) | `Rendering/GridItemList.cpp` | `GridItemList.cpp drawSpriteCommandRight()` |
| ✅ | Icon tint uses sprite colour (`kFocusTexture`/`kUnfocusedTexture`) | `Rendering/GridItemList.cpp` | `g_renderer.getFocusSpriteColour()` |
| ✅ | List `indicator_type` (ARROW / ARROW_IF_CHILDREN / OFF / ON) | `Commands/Widgets/CommandList.hpp`, `Rendering/GridItemList.cpp` | `CommandList.hpp indicator_type` |
| ✅ | `COMMAND_LIST_SEARCH` → Search icon | `Commands/Widgets/Command.hpp`, `Rendering/GridItemList.cpp` | `GridItemList.cpp COMMAND_LIST_SEARCH` |
| ✅ | `COMMAND_INPUT` → Edit icon | `Commands/Widgets/Command.hpp`, `Rendering/GridItemList.cpp` | `GridItemList.cpp CommandInput` |
| ✅ | `COMMAND_READONLY_LINK` → Link icon | `Commands/Widgets/Command.hpp`, `Rendering/GridItemList.cpp` | `GridItemList.cpp CommandReadonlyLink` |
| ✅ | Cursor border around focused row (`kCursorBorderWidth/Colour`) | `Rendering/GridItemList.cpp draw()` | `g_renderer.drawBorderH/C()` |
| ✅ | `CommandListSelect` shows `< value >` only when focused | `Rendering/GridItemList.cpp` | `GridItemList.cpp COMMAND_LIST_SELECT` |
| ❌ | CJK font fallback (Yahei / NanumGothic for JP/KR/CN) | — | `Renderer.hpp` (3 SpriteFont instances) |
| ✅ | Background blur behind menu panels | `Rendering/BackgroundBlur.hpp/cpp` | DX12 port: `CopyTextureRegion` back-buffer capture + `BasicPostProcess::GaussianBlur_5x5` ping-pong; `GridItem::bgblur` + `drawBackgroundBlur()`; `GridItemHeaderAnimation::draw()` calls it when `HeaderBanner::GetBgBlur()` |

---

## Core Grid & GridItem System

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | `Grid` base class | `Rendering/StandPort/Position2d.hpp/cpp` | `Grid.hpp/cpp` |
| ✅ | `MenuGrid` (header + sidebar + content chrome) | `Rendering/MenuGrid.hpp/cpp` | `MenuGrid.hpp/cpp` |
| ✅ | `GridItemList` (command list two-pass renderer) | `Rendering/GridItemList.hpp/cpp` | `GridItemList.hpp/cpp` |
| ✅ | `GridItemScrollbar` | `Rendering/GridItemScrollbar.hpp/cpp` | `GridItemScrollbar.hpp/cpp` |
| ✅ | `GridItemAddressbar` | `Rendering/GridItemAddressbar.hpp/cpp` | `GridItemAddressbar.hpp/cpp` |
| ✅ | `GridItemTabsVertical` (sidebar) | `Rendering/GridItemTabsVertical.hpp/cpp` | `GridItemTabsVertical.hpp/cpp` |
| ✅ | `GridItemTabsHorizontal` | `Rendering/GridItemTabsHorizontal.hpp/cpp` | `GridItemTabsHorizontal.hpp/cpp` |
| ✅ | `GridItemText` | `Rendering/GridItemText.hpp/cpp` | `GridItemText.hpp/cpp` |
| ❌ | `GridItemPrimaryText` (abstract base for address/header text — to port explicitly later) | — | `GridItemPrimaryText.hpp/cpp` — functionality inlined in `GridItemAddressbar` for now; needs proper port so subclasses can inherit it |
| ✅ | `GridItemTextBigCentre` + Bordered variant | `Rendering/GridItemTextBigCentre.hpp/cpp`, `Rendering/GridItemTextBigCentreBordered.hpp/cpp` | `GridItemTextBigCentre.hpp/cpp`, `GridItemTextBigCentreBordered.hpp/cpp` |
| ✅ | `GridItemColourBox` + Bordered variant (colour palette) | `Rendering/GridItemColourBox.hpp/cpp`, `Rendering/GridItemColourBoxBordered.hpp/cpp` | `GridItemColourBox.hpp/cpp`, `GridItemColourBoxBordered.hpp/cpp` |
| ✅ | `GridItemCommandboxInput` (word-wrapped text field for command box) | `Rendering/GridItemCommandboxInput.hpp/cpp` | `GridItemCommandboxInput.hpp/cpp` |
| ✅ | `GridItemQrcode` (QR code via `soup::Canvas`) | `Rendering/GridItemQrcode.hpp/cpp` | `GridItemQrcode.hpp/cpp` |
| ✅ | `GridItemHeaderLoading` (progress overlay text + loading sprite during header load) | `Rendering/GridItemHeaderLoading.hpp/cpp`, `Rendering/HeaderLoadingSprite.hpp/cpp` | `GridItemHeaderLoading.hpp/cpp` — `header_goal/progress` → `HeaderBanner::kLoadingGoal/Progress`; height + sprite from `HeaderLoadingSprite`; queued via `GridRenderer::QueueHeaderLoadingDraw` |
| ✅ | `GridItemHeader` (abstract base for all header items) | `Rendering/GridItemHeader.hpp/cpp` | `GridItemHeader.hpp/cpp` |
| ✅ | `GridItemHeaderAnimation` (animated PNG frame header) | `Rendering/GridItemHeaderAnimation.hpp/cpp` | `GridItemHeaderAnimation.hpp/cpp` — height from `HeaderBanner::GetRenderHeight`; frame draw via `GridRenderer::QueueHeaderDraw` (flushed by DrawImpl) |
| ✅ | `GridItemNotify` (standalone timed notification card class) | `Rendering/GridItemNotify.hpp/cpp` | `GridItemNotify.hpp/cpp` — inherits `GridItem` (OSS `GridItemText` interface differs); `notifyBorder/Bg/Flash` → `NotifySettings`; `g_notify_grid.update()` → `on_expire` callback; freeze: `NotifySettings::kFrozenSince` + static instance registry + `Freeze()`/`Unfreeze()` |

---

## Navigation & Focus

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | `MenuNavigation` (drill-down stack) | `Rendering/MenuNavigation.hpp/cpp` | `MenuNavigation.hpp/cpp` |
| ✅ | `MenuFocus` (sidebar vs content region tracking) | `Rendering/MenuFocus.hpp/cpp` | `MenuFocus.hpp/cpp` |
| ✅ | `Gui` (root state: command_rows, lerp, warnings) | `Rendering/Gui.hpp/cpp` | `Gui.hpp/cpp` |
| ✅ | Keyboard navigation (Up/Down/Left/Right/Enter/Back) | `Rendering/MenuGrid.cpp`, `Rendering/GridItemListGrid.cpp` | — |
| ✅ | Mouse navigation (hover focus, click, scroll wheel) | `Rendering/MenuGrid.cpp` | — |
| ✅ | Sidebar navigation (RCtrl down / RShift up) | `Rendering/MenuGrid.cpp` | — |
| ✅ | Scroll-to-show focused item | `Rendering/Grid.cpp` `ScrollToShow()` | — |
| ✅ | Per-tab navigation stack save/restore | `Rendering/MenuNavigation.hpp/cpp` | — |
| ✅ | Activation of CommandList subcommands (Enter) | `Rendering/GridItemListGrid.cpp` `activateFocused()` | — |
| 🔶 | Lerp animation (smooth scroll between rows) | `Rendering/GridItemList.cpp`, `Rendering/Gui.hpp` | `Gui.hpp` — easing curve may differ |
| ✅ | **Context menu per command (O key)** — full 1:1 port | `Menu/ContextMenu.hpp/cpp`, `Commands/Context/CommandCtx*.hpp/cpp`, `Commands/Context/CommandQuickCtx*.hpp/cpp` | `ContextMenu.hpp/cpp`, `CommandCtx*.hpp` |

---

## Command Box & Search

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | Command box open/close (hotkey) | `Rendering/MenuCommandBox.hpp/cpp` | — |
| ✅ | Autocomplete / live match list | `Rendering/MenuCommandConsole.hpp/cpp` | — |
| ❌ | **Full Textbox** (history ↑↓, IME, Ctrl+Left/Right word-jump, selection) | — | `Textbox.hpp/cpp`, `TextboxInterface.hpp/cpp` |
| ❌ | Colour selector in command box (11-colour palette) | — | `Commandbox.hpp`, `CommandboxGrid.hpp`, `GridItemColourBox.hpp` |
| ❌ | `CommandSearch` / `CommandSearchMenu` / `CommandSearchPlayer` | — | `CommandSearch.hpp/cpp`, `CommandSearchMenu.hpp/cpp`, `CommandSearchPlayer.hpp/cpp` |

---

## Context Menu (per-command, O key)

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | Context menu shell (`ContextMenu::open/close/toggle`) | `Menu/ContextMenu.hpp/cpp` | `ContextMenu.hpp/cpp` |
| ✅ | Save / Load state actions (conditional: not CMDFLAG_TEMPORARY, supportsSavedState) | `Menu/ContextMenu.cpp` `doSaveState`/`doLoadState` | `ContextMenu.cpp` |
| ✅ | Apply Default State to Children (list commands) | `Menu/ContextMenu.cpp` `doApplyDefaultStateToChildren` | `ContextMenu::doApplyDefaultStateToChildren` |
| ✅ | Slider Min / Max (slider commands only) | `Menu/ContextMenu.cpp` `doMin`/`doMax` | `ContextMenu::doMin/doMax` |
| ✅ | `getStateCommand()` on `CommandPhysical` | `Commands/Widgets/CommandPhysical.hpp/cpp` | `CommandPhysical::getStateCommand()` |
| ✅ | `isListNonAction()` / `canBeResolved()` on `Command` | `Commands/Widgets/Command.hpp` | `Command.hpp` |
| ✅ | Context menu item order matches OSS (correlation → state ops → list children → slider min/max → hotkeys → star → address) | `Menu/ContextMenu.cpp` `open()` | `ContextMenu::open()` |
| ✅ | **Hotkey list** (view / add / edit / remove per-command hotkeys) | `Commands/Context/CommandCtxHotkeys.hpp/cpp`, `Commands/Context/CommandCtxHotkey.hpp/cpp`, `Commands/Context/CommandCtxHotkeyRemove.hpp` | `CommandCtxHotkeys.hpp/cpp`, `CommandCtxHotkey.hpp/cpp`, `CommandCtxHotkeyRemove.hpp` |
| ✅ | Hold-mode toggle per hotkey | `Commands/Context/CommandCtxHotkeyHoldMode.hpp` | `CommandCtxHotkeyHoldMode.hpp` |
| ✅ | Copy address to clipboard (4 modes: User / Default / API / Link) | `Commands/Context/CommandCtxAddress.hpp` | `CommandCtxAddress.hpp` |
| ✅ | Star / favourite a command | `Commands/Context/CommandCtxStar.hpp`, `Rendering/Gui.hpp` `starred_commands` | `CommandCtxStar.hpp` |
| ✅ | Toggle correlation — flat root-level items (type selector + invert) | `Commands/Context/CommandCtxToggleCorrelation.hpp/cpp`, `Commands/Context/CommandCtxToggleCorrelationInvert.hpp` | `CommandCtxToggleCorrelation.hpp/cpp`, `CommandCtxToggleCorrelationInvert.hpp` |
| ✅ | Quick-context actions (Save/Load/Default/RDefault/Min/Max as hotkey-triggered commands) | `Commands/Context/CommandQuickCtx*.hpp` | `CommandQuickCtx*.hpp` |

---

## Hotkeys & Input

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | Hotkey binding (VK + Ctrl/Shift/Alt modifiers) | `Config/HotkeySystem.hpp/cpp` | — |
| ✅ | Hotkey persistence (save/load) | `Config/HotkeySystem.hpp/cpp` | — |
| ✅ | `HotkeysGrid` (settings page for all hotkeys) | `Rendering/HotkeysGrid.hpp/cpp` | — |
| ✅ | Hold-mode hotkeys | `Config/HotkeySystem.hpp/cpp` | — |
| ✅ | Controller input config | `Rendering/Theme.hpp`, `Commands/Settings/Input/` | — |
| ✅ | `InputCapture` (gate hotkeys while text field active) | `Rendering/InputCapture.hpp` | — |
| ✅ | Keyboard layout presets (numpad / no-numpad) | `Commands/Settings/Input/CommandTabInput.hpp` | — |
| ❌ | `ButtonInstructions` (GTA scaleform key-hint overlay) | — | `ButtonInstructions.hpp/cpp` |

---

## Theme & Styling

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | Colour tokens (accent, background, text, etc.) | `Rendering/Theme.hpp/cpp` | — |
| ✅ | Theme load/save from disk | `Rendering/Theme.hpp/cpp` | — |
| ✅ | Rainbow accent colour animation | `Rendering/RainbowColor.hpp` | — |
| ✅ | Menu position (X/Y on screen) | `Rendering/SettingsPositionGrid.hpp/cpp` | — |
| ✅ | Menu height (visible command rows) | `Rendering/SettingsPositionGrid.hpp/cpp` | — |
| ✅ | Tab position (Left/Right/Top/Bottom) | `Rendering/SettingsTabsGrid.hpp/cpp` | — |
| ✅ | Address bar settings (separator, current-list-only, width) | `Rendering/SettingsAddressBarGrid.hpp/cpp` | — |
| ✅ | Font settings (scale, small text scale) | `Rendering/SettingsFontTextGrid.hpp/cpp`, `SettingsPresetFontGrid.hpp/cpp` | — |
| ✅ | Border / cursor settings | `Rendering/SettingsBorderGrid.hpp/cpp`, `SettingsCursorGrid.hpp/cpp` | — |
| ✅ | Header banner (animated PNG sequence) | `Rendering/HeaderBanner.hpp/cpp` | — |
| ✅ | Stream-proof rendering suppression | `Commands/Settings/Appearance/CommandStreamproof.hpp` | — |

---

## Overlays & HUD

| Status | System | Our File(s) | OSS Reference |
|--------|--------|-------------|---------------|
| ✅ | Notification overlay (timed toast cards) | `Rendering/Notifications.hpp/cpp` | — |
| ✅ | Notification settings (position, colour, type, timing) | `Rendering/SettingsNotificationsGrid.hpp/cpp` | — |
| ✅ | Status overlay (FPS, position, business info) | `Rendering/Overlay.hpp/cpp` | — |
| ✅ | Chat display overlay | `Rendering/ChatDisplay.hpp/cpp` | — |
| ✅ | ESP (player/NPC outlines, names, bones, boxes, lines) | `Rendering/ESP.hpp/cpp` | — |
| ✅ | AR overlays (speed, waypoint, GPS route) | `Commands/Vehicle/ARSpeed/`, `Commands/World/CommandArWaypoint`, `CommandArGps` | — |
| ✅ | `MenuPopup` (Yes/No confirm modal) | `Rendering/MenuPopup.hpp/cpp` | — |
| ✅ | Onboarding (first-run setup screen) | `Rendering/Onboarding.hpp/cpp` | — |
| ✅ | Description panel (focused command help text) | `Rendering/DescriptionPanel.hpp/cpp`, `Rendering/GridItemStandCommand.cpp::GetDescription()` | OSS equivalent: `CommandPhysical::populateCorner()` + `GridItemText` items in `MenuGrid.cpp`. Our arch uses a free-standing overlay + `GetDescription()` per `GridItem`. Content now matches OSS: help text, syntax, slider range (no-cmd-name case), ListSelect value help text. Skipped: `is_click_to_apply`, toggle correlation `getExplanation()`, permission labels, Lua ownership — infra not yet ported. |
| ❌ | AR notification popups above player/NPC heads | — | `CommandArNotifications.hpp/cpp` |
| ❌ | `CommandBirender` (DX vs native renderer toggle for ESP) | — | `CommandBirender.hpp/cpp`, `AbstractRenderer.hpp/cpp` |
| 🔶 | Expanded info overlay (wanted, RP, money, session fields) | `Commands/Extra/CommandListInfoOverlay.hpp/cpp`, `Rendering/Overlay.hpp/cpp` | `CommandListInfoOverlay.hpp/cpp` — 17 of ~30 fields ported; OSS-only fields (wanted, RP, money, vehicle speed unit, session type/id/region) require infra not yet ported |
| ❌ | Toaster / `GridToaster` (decoupled toast interface) | — | `Toaster.hpp/cpp`, `GridToaster.hpp/cpp` |
| ❌ | `FmBanner` / `CommandFmBanner2Notify` (GTA banner interception) | — | `FmBanner.hpp/cpp`, `CommandFmBanner2Notify.hpp` |
| ❌ | `TutorialGrid` (first-launch interactive key-teaching wizard) | — | `TutorialGrid.hpp/cpp`, `Tutorial.hpp/cpp` |

---

## Settings Grids (all done)

| Status | System | Our File(s) |
|--------|--------|-------------|
| ✅ | All Settings > Appearance grids | `Rendering/SettingsAppearanceGrid`, `SettingsColoursGrid`, `SettingsHeaderGrid`, `SettingsBorderGrid`, `SettingsCursorGrid` |
| ✅ | All Settings > Input grids | `Rendering/SettingsInputGrid`, `SettingsInputKeyboardGrid`, `SettingsInputMouseGrid`, `SettingsInputPresetsGrid`, `SettingsInputControllerSchemeGrid`, `SettingsInputContextHotkeysGrid`, `SettingsInputKeepCursorGrid` |
| ✅ | All Settings > Notifications grids | `Rendering/SettingsNotificationsGrid`, `SettingsNotifyPositionGrid`, `SettingsNotifySampleGrid`, `SettingsNotifyTimingGrid` |
| ✅ | Settings > Position, Font, Commands, Scrollbar | `Rendering/SettingsPositionGrid`, `SettingsFontTextGrid`, `SettingsPresetFontGrid`, `SettingsCommandsGrid`, `SettingsScrollbarGrid` |
| ✅ | Settings > Profiles, Entity Previews, Textures | `Rendering/SettingsProfilesGrid`, `SettingsEntityPreviewsGrid`, `SettingsTexturesGrid` |

---

## Cannot Port (DX11-specific)

| System | Reason |
|--------|--------|
| ~~`BackgroundBlur.hpp/cpp`~~ | Ported as DX12 — see BackgroundBlur entry above. |
| `AbstractRendererNative.hpp/cpp` | GTA native draw path (`drawLine`, `drawRect` via scaleform). Only useful if mixing GTA-native and DX draws; pure DX12 path is preferable. |
