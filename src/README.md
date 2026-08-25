# `src/` layout

Stand's own source used to live as ~2,500 flat `.cpp`/`.hpp` files directly
under `src/`. They're now grouped into subfolders by area, one `.cpp` and
its matching `.hpp` always sitting side by side in the same folder. This
is a physical reorganization only - build behavior, include resolution,
and namespaces are unchanged. See the "How includes still work" section
below if you're moving files around further.

## Layout

- **`Commands/`** - every menu command (`Command*.hpp`/`.cpp`), split by
  the same tabs the in-game menu itself uses:
  - `Self/`, `Player/`, `Vehicle/`, `World/`, `Online/` - the five main
    per-domain command tabs.
  - `Stand/` - menu/app settings: hotkeys, colours, fonts, profiles,
    notification style, etc.
  - `Extra/` - one-off game features: DLC, casino, heists, session
    scripts, easter eggs, dev/debug tools.
  - `Weapons/`, `Ped/` - weapon- and pedestrian-specific commands.
  - `Widgets/` - the reusable command base classes the menu is built out
    of (toggles, sliders, lists, colour pickers, text inputs, ...) rather
    than a specific feature.
- **`Core/`** - process bootstrap, the DLL/ASI entry point, threading,
  allocation, fibers, ticking, the event system, and other engine-agnostic
  runtime plumbing. Also holds `common.hpp`, Stand's precompiled header.
- **`Hooking/`** - function hooking (`DetourHook`, `Hooking`,
  `NativeFingerprinter`, native table hooks), pattern scanning, and the
  detection-evasion/obfuscation code built on top of it (anti-debug,
  anti-anti-cheat, code integrity, string/hash obfuscation).
- **`Game/`** - reverse-engineered RAGE/GTA V engine types: native
  wrappers, enums, and engine data structures (`C*`, `e*`, `gta_*`, `at*`,
  `fw*`, etc.).
- **`Network/`** - sessions, matchmaking, the Rockstar Social Club, chat,
  BattlEye, and player-history/social-club-identity tracking.
- **`Vehicle/`**, **`Ped/`**, **`Weapons/`**, **`World/`** - non-command
  support classes for each of those domains.
- **`Rendering/`** - the renderer, GUI drawing, textures, colours, and
  fonts.
- **`Menu/`** - the menu's own UI framework: grids, toasts, the tutorial,
  hotkeys, context menu, click handling.
- **`Scripting/`** - the Lua/script-VM integration layer.
- **`Config/`** - settings persistence: config trees, tunables, packages,
  stats, profiles.
- **`Localization/`** - translated strings and language data.
- **`Util/`** - generic, domain-agnostic helpers (containers, math,
  string/time formatting, small OS helpers).
- **`lib/`** - vendored third-party code: `pluto/` (the Lua/Pluto script
  VM), `discord-rpc/` (Discord Rich Presence), `soup/` (a general-purpose
  C++ toolkit used throughout Stand's own code, e.g. `<soup/Rgb.hpp>`),
  `souplua/` (soup's Lua bindings), `fmt/`, `rapidjson/`, `minhook/`,
  `stackwalker/`, and `directxtk/` (including its precompiled shader
  blobs, under `directxtk/shaders/`). Everything here is committed
  source, not fetched at configure time - see CMakeLists.txt for exactly
  which files each one compiles.

## How includes still work

Every file here still `#include`s its neighbours by bare filename (e.g.
`#include "Renderer.hpp"`, never `#include "Rendering/Renderer.hpp"`).
That works because CMakeLists.txt adds *every* directory under `src/`
that contains a header to the include search path (see
`STAND_INCLUDE_DIRS` in CMakeLists.txt) - so wherever a header ends up,
the compiler can still find it by name alone. `lib/soup/` and
`lib/souplua/` are deliberately left out of that search path: a few of
soup's headers happen to share a bare name with an unrelated Stand
header (both have their own `Worker.hpp`, for instance), and code that
wants a soup/souplua/discord-rpc header always spells it out qualified -
`"soup/Foo.hpp"` or `<soup/Foo.hpp>` - which resolves instead via
`src/lib/` itself being on the path (see `LIB_DIR` in CMakeLists.txt), so
nothing needs `lib/soup/`'s or `lib/souplua/`'s own subdirectories added.

Practical implications:
- Adding a new file: drop it in whichever folder above fits, matching
  `.cpp`/`.hpp` together. No CMake changes needed - sources are picked up
  by `file(GLOB_RECURSE ...)`.
- Moving a file to a different folder: safe on its own. Just keep the
  `.cpp`/`.hpp` pair together, and avoid introducing a new file whose bare
  name collides with another Stand header (a Stand-vs-Stand collision,
  unlike the soup/ case above, *would* be genuinely ambiguous).

## Editing in VS Code

See the repo-root README for editor/build setup - CMake Tools + clangd,
driven by `CMakePresets.json`, gives full cross-platform IntelliSense and
a build-the-DLL command without needing the full Visual Studio IDE.
