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

Vendored/third-party code (`pluto/`, `discord-rpc/`, `soup/`, `souplua/`)
is untouched by this reorganization and keeps its existing layout.

## How includes still work

Every file here still `#include`s its neighbours by bare filename (e.g.
`#include "Renderer.hpp"`, never `#include "Rendering/Renderer.hpp"`).
That works because CMakeLists.txt adds *every* directory under `src/`
that contains a header to the include search path (see
`STAND_INCLUDE_DIRS` in CMakeLists.txt) - so wherever a header ends up,
the compiler can still find it by name alone. `soup/` and `souplua/` are
deliberately left out of that search path: a few of their headers happen
to share a bare name with an unrelated Stand header (both have their own
`Worker.hpp`, for instance), and code outside those folders that wants a
soup header always spells it `"soup/Foo.hpp"` rather than a bare name, so
nothing needs them added.

Practical implications:
- Adding a new file: drop it in whichever folder above fits, matching
  `.cpp`/`.hpp` together. No CMake changes needed - sources are picked up
  by `file(GLOB_RECURSE ...)`.
- Moving a file to a different folder: safe on its own. Just keep the
  `.cpp`/`.hpp` pair together, and avoid introducing a new file whose bare
  name collides with another Stand header (a Stand-vs-Stand collision,
  unlike the soup/ case above, *would* be genuinely ambiguous).
