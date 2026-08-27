# `src/` layout

Stand's own source used to live as ~2,500 flat `.cpp`/`.hpp` files directly
under `src/`. They're now grouped into subfolders by area, one `.cpp` and
its matching `.hpp` always sitting side by side in the same folder, and
every `#include` is written out fully qualified from `src/` itself - e.g.
`#include "Game/CObject.hpp"`, never a bare `#include "CObject.hpp"`. See
the "How includes work" section below for why, and what that means if
you're moving files around further.

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
- **`AntiCheat/`** - detection evasion, code integrity, function hooking,
  pattern scanning, and obfuscation.
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
  C++ toolkit used throughout Stand's own code, e.g.
  `#include "lib/soup/Rgb.hpp"`), `souplua/` (soup's Lua bindings), `fmt/`,
  `rapidjson/`, `minhook/`, `stackwalker/`, and `directxtk/`. Everything
  here is committed source, not fetched at configure time - see
  CMakeLists.txt for exactly which files each one compiles.
  `directxtk/shaderscompiled/` holds its shaders as already-compiled
  `.inc` byte arrays (what actually gets built by default -
  `directxtk/shaders/` is only their HLSL source, and is empty unless
  you're using `STAND_COMPILE_DIRECTXTK_SHADERS` - see
  cmake/directxtk-shaders.cmake - to regenerate them).

## How includes work

Every file here `#include`s another Stand header (or a vendored header
under `lib/`) by its full path relative to `src/` itself - e.g.
`#include "Game/CObject.hpp"` from anywhere, or `#include "lib/soup/rand.hpp"`
to reach into a vendored library - never a bare `#include "CObject.hpp"`.
CMakeLists.txt (and `.clangd`, for editors) only need `src/` itself on the
include search path for any of this to resolve, because the path is
always spelled out in full; there's no per-directory list to keep in
sync, and no ambiguity when two folders happen to contain a same-named
header (`Core/Worker.hpp` and `lib/soup/Worker.hpp` are unrelated classes
that coexist just fine).

A handful of vendored libraries are the one exception, reached instead
the way any third-party library normally is - unqualified, by their own
public header name (`<fmt/core.h>`, `<rapidjson/document.h>`,
`<MinHook.h>`, `<lua.h>`, DirectXTK's own headers) via their own include
root in CMakeLists.txt. `soup`, `souplua`, `discord-rpc`, and
`stackwalker` don't get that treatment - they're reached fully qualified
like Stand's own code (`"lib/soup/Foo.hpp"`, `"lib/discord-rpc/include/discord_rpc.h"`,
`"lib/stackwalker/StackWalker.h"`) instead.

Practical implications:
- Adding a new file: drop it in whichever folder above fits, matching
  `.cpp`/`.hpp` together, and `#include` it (from elsewhere) by that full
  path. No CMake changes needed - sources are picked up by
  `file(GLOB_RECURSE ...)`.
- Moving a file to a different folder: update every `#include` that
  spells out its old path (a project-wide search for the filename finds
  them all) along with the file itself.
- Within `lib/`, each vendored library keeps whatever include style it
  arrived with internally (soup's own files still `#include` each other
  by bare filename, for instance) - only how *Stand's own code* reaches
  into a library follows the rule above.

## Editing in VS Code

See the repo-root README for editor/build setup - CMake Tools + clangd,
driven by `CMakePresets.json`, gives full cross-platform IntelliSense and
a build-the-DLL command without needing the full Visual Studio IDE.
