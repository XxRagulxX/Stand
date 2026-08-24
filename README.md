# Stand

See `src/README.md` for how the source tree under `src/` is organized.

## Building

The project is CMake-based (no `.sln`/`.vcxproj` to open - those were
Visual Studio artifacts and have been removed). It builds `Stand.dll` two
ways:

- **On Linux**, by default: cross-compiles for Windows using clang-cl
  (MSVC ABI) via `cmake/cross-compile.cmake`. Requires LLVM and
  msvc-wine.
- **On Windows**, natively, with MSVC (`cl.exe`) or Clang's MSVC-compatible
  driver (`clang-cl.exe`) - see `cmake/windows.cmake`. Either one comes
  from **Visual Studio Build Tools** or a standalone **LLVM** install plus
  the Windows SDK - not the full Visual Studio IDE.

`CMakePresets.json` defines a preset for each combination
(`linux-cross-debug`/`-release`, `windows-debug`/`-release`).

## Editing in VS Code

VS Code + two extensions covers both editing and building, on Windows and
Linux alike, without Visual Studio:

- **[CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)**
  - configures and builds `Stand.dll` from inside the editor (`CMake:
    Select Configure Preset`, then `CMake: Build`). On Windows, also pick
    a matching **kit** first (`CMake: Select a Kit`) - e.g. "Visual
    Studio Build Tools 2022 - amd64" or an LLVM/clang-cl kit - so
    `cl.exe`/`clang-cl.exe` and the Windows SDK are on the environment;
    CMake Tools applies that automatically on top of the preset. Nothing
    extra is needed on Linux, since the default preset there just works.
- **[clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)**
  - drives IntelliSense (go to definition, completion, diagnostics) from
    `build/compile_commands.json` (path configured in `.clangd`), which
    CMake regenerates on every configure with the exact include paths,
    defines, and flags actually used - so headers resolve correctly on
    both platforms without a hand-maintained include list. Re-run `CMake:
    Configure` after switching presets/kits so this file reflects the
    toolchain you're building with.

Both are listed in `.vscode/extensions.json` as workspace recommendations
- VS Code will prompt to install them on first open. See
`.vscode/settings.json` and `.clangd` for how they're configured.

### Making clangd work on both platforms

clangd only ever knows what `build/compile_commands.json` says (plus
`.clangd`'s own additions). A few things about how Stand builds make that
incomplete or wrong unless worked around; all of them already are, below
- but it helps to know about them when something still doesn't resolve.

**Everything is red / most headers unresolved, despite a successful
build**: this almost always means clangd is running against stale state,
not a real bug in the project. In order:

1. Make sure your checkout actually has these fixes -
   `git log --oneline -1 -- .clangd` should show a recent commit. A stale
   checkout means none of the below applies yet.
2. Delete the build directory and reconfigure from scratch:
   `rm -rf build`, then **CMake: Configure** (or `cmake --preset
   linux-cross-debug` / `windows-debug` from a terminal). This regenerates
   `build/compile_commands.json` against the current CMakeLists.txt -
   important since a compile database from before a CMake change (a
   moved/renamed vendored library, for instance) is silently wrong rather
   than missing.
3. Restart clangd itself: **clangd: Restart language server** (or reload
   the VS Code window). clangd only reads `.clangd` and the compile
   database when it starts/restarts, not on every keystroke.

If it's still broken after all three, something's genuinely wrong - see
the specific failure modes below.

**Unity build.** When turned on, CMake compiles Stand as a handful of
synthesized `Unity/unity_N_cxx.cxx` files, each `#include`-ing a batch of
the real `.cpp` files - and `compile_commands.json` only ever gets an
entry for what actually gets compiled, so under a unity build, individual
files like `CommandOnPlayer.cpp` have **no entry at all**. clangd's usual
trick for a header with nothing of its own in the database
(`CommandOnPlayer.hpp`, which is never its own translation unit) is to
borrow its sibling `.cpp`'s flags; with no such entry to borrow, it falls
back to a generic guess that knows nothing about this project - hence
errors like `fmt/core.h` (or any other vendored/Stand header) not being
found, in effectively any header. `STAND_UNITY_BUILD` therefore
**defaults to OFF** - accurate per-file `compile_commands.json` entries,
which is what any plain `cmake -S . -B build`, any IDE's own default
configure, and clangd all need - and only the `-release` presets in
`CMakePresets.json` explicitly turn it back on, for a full CI/from-scratch
rebuild where the speed is worth losing that per-file accuracy. If you
ever do need to check, `cmake -L build | grep STAND_UNITY_BUILD` should
say `OFF`.

**Windows system headers**: clangd never inherits VS Code's (or any
launching shell's) environment. That's a problem specifically for the
CRT/STL/Windows SDK headers, which
cl.exe/clang-cl.exe normally find through the `INCLUDE` environment
variable (set by `vcvarsall.bat`, a Developer Command Prompt, or a CMake
Tools kit) rather than an explicit flag - an env var that's long gone by
the time clangd itself tries to parse a file. Both build paths avoid this
the same way, by turning `INCLUDE` into explicit, self-contained flags on
every compile command *at configure time*, before it's lost:

- **Linux → Windows cross-compile**: `cmake/cross-compile.cmake` already
  resolves the msvc-wine MSVC/Windows SDK install into explicit `/imsvc`
  flags (needed for the real build regardless of clangd).
- **Native Windows**: `cmake/windows.cmake` reads the active `INCLUDE`
  environment variable at configure time (present because you configured
  from a Developer Command Prompt, or via a CMake Tools kit) and bakes
  each of its directories into an explicit flag the same way - `/imsvc`
  for clang-cl, `/I` for cl.exe (which doesn't understand `/imsvc`).
  Configuring without `INCLUDE` set prints a `CMake Warning` explaining
  this.

If you ever see clangd failing to find `<windows.h>` or similar, the fix
is almost always: re-run **CMake: Configure** (with a kit selected on
Windows) so `build/compile_commands.json` gets regenerated with those
flags baked in, then reload the clangd extension (**clangd: Restart
language server**).

**Headers with no compile command of their own.** Most of Stand's own
`.hpp` files are never a translation unit by themselves - clangd handles
that by borrowing the flags of a same-named `.cpp` (`Foo.hpp` borrows
`Foo.cpp`'s), or from whatever it's already indexed as including that
header. Neither always kicks in immediately (a header-only file with no
matching `.cpp`, or simply background-indexing not having reached it
yet), and when it doesn't, clangd falls back to a generic guess with none
of this project's include paths - which is what was behind `<soup/...>`,
`<fmt/...>` and similar failing to resolve even though the real build (and
most other files) worked fine. `.clangd`'s `CompileFlags.Add` covers this
directly: every one of Stand's own top-level `src/` subfolders, `src/lib/`
itself, and each vendored library's own public include directory are
appended to *every* file clangd compiles, compile command or not - so
this doesn't depend on which heuristic clangd happened to fall back to,
on either platform. That list is spelled out statically in `.clangd`
(clangd can't run CMake's own directory-discovery logic itself), so it
needs a manual update if `src/`'s top-level layout ever changes - see the
comment above `Add:` in `.clangd`.

`.clangd` also turns off clangd's IncludeCleaner diagnostics
(`MissingIncludes`/`UnusedIncludes`): this codebase leans on a shared
precompiled header (`src/Core/common.hpp`) to pull in most of what any
given file needs, which IncludeCleaner isn't aware of, so it would
otherwise flag a lot of headers as "missing" or "unused" that aren't
actually either.
