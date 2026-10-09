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
| ➖ | Background blur behind menu panels | — | `BackgroundBlur.hpp/cpp` — DX11 capture; DX12 UAV rewrite required |

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
| ❌ | `GridItemPrimaryText` (abstract base for address/header text) | — | `GridItemPrimaryText.hpp/cpp` |
| ❌ | `GridItemTextBigCentre` + Bordered variant | — | `GridItemTextBigCentre.hpp/cpp`, `GridItemTextBigCentreBordered.hpp/cpp` |
| ❌ | `GridItemColourBox` + Bordered variant (colour palette) | — | `GridItemColourBox.hpp/cpp`, `GridItemColourBoxBordered.hpp/cpp` |
| ❌ | `GridItemCommandboxInput` (typed-text field with cursor blink) | — | `GridItemCommandboxInput.hpp/cpp` |
| ❌ | `GridItemQrcode` (QR code via `soup::Canvas`) | — | `GridItemQrcode.hpp/cpp` |
| ❌ | `GridItemHeaderLoading` (progress bar in header during load) | — | `GridItemHeaderLoading.hpp/cpp` |
| ❌ | `GridItemHeader` (abstract base for all header items) | — | `GridItemHeader.hpp/cpp` |
| ❌ | `GridItemHeaderAnimation` (animated PNG frame header) | — | `GridItemHeaderAnimation.hpp/cpp` |
| ❌ | `GridItemNotify` (standalone timed notification card class) | — | `GridItemNotify.hpp/cpp` |

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
| 🔶 | **Context menu per command (O key)** — shell works; hotkey/star/address/correlation missing | `Rendering/CommandContextGrid.hpp/cpp` | `ContextMenu.hpp/cpp`, `CommandCtx*.hpp` |

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
| ✅ | Context menu shell (opens `CommandContextGrid`) | `Rendering/CommandContextGrid.hpp/cpp` | — |
| ✅ | Save / Load state actions | `Rendering/CommandContextGrid.hpp/cpp` | — |
| ❌ | **Hotkey list** (view / add / edit / remove per-command hotkeys) | — | `CommandCtxHotkeys.hpp/cpp`, `CommandCtxHotkey.hpp/cpp`, `CommandCtxHotkeyRemove.hpp` |
| ❌ | Hold-mode toggle per hotkey | — | `CommandCtxHotkeyHoldMode.hpp` |
| ❌ | Copy address to clipboard | — | `CommandCtxAddress.hpp` |
| ❌ | Star / favourite a command | — | `CommandCtxStar.hpp` |
| ❌ | Toggle correlation (link two toggles) | — | `CommandCtxToggleCorrelation.hpp/cpp` |

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
| ✅ | Description panel (focused command help text) | `Rendering/DescriptionPanel.hpp/cpp` | — |
| ❌ | AR notification popups above player/NPC heads | — | `CommandArNotifications.hpp/cpp` |
| ❌ | `CommandBirender` (DX vs native renderer toggle for ESP) | — | `CommandBirender.hpp/cpp`, `AbstractRenderer.hpp/cpp` |
| ❌ | Expanded info overlay (wanted, RP, money, session fields) | — | `CommandListInfoOverlay.hpp/cpp` |
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
| `BackgroundBlur.hpp/cpp` | DX11 render-target capture + multi-pass box blur. DX12 equivalent needs a UAV/SRV resolve pass on the swap-chain back buffer — separate rewrite required. |
| `AbstractRendererNative.hpp/cpp` | GTA native draw path (`drawLine`, `drawRect` via scaleform). Only useful if mixing GTA-native and DX draws; pure DX12 path is preferable. |
