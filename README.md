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

clangd only knows what `build/compile_commands.json` tells it - it never
inherits VS Code's (or any launching shell's) environment. That's a
problem specifically for the CRT/STL/Windows SDK headers, which
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

`.clangd` also turns off clangd's IncludeCleaner diagnostics
(`MissingIncludes`/`UnusedIncludes`): this codebase leans on a shared
precompiled header (`src/Core/common.hpp`) to pull in most of what any
given file needs, which IncludeCleaner isn't aware of, so it would
otherwise flag a lot of headers as "missing" or "unused" that aren't
actually either.
