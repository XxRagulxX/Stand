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
    `build/compile_commands.json`, which CMake regenerates on every
    configure with the exact include paths, defines, and flags actually
    used - so headers resolve correctly on both platforms without a
    hand-maintained include list. Re-run `CMake: Configure` after
    switching presets/kits so this file reflects the toolchain you're
    building with.

Both are listed in `.vscode/extensions.json` as workspace recommendations
- VS Code will prompt to install them on first open. See
`.vscode/settings.json` for how they're wired together.
