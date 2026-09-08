# Run once as this FetchContent dependency's own PATCH_COMMAND (see
# cmake/directxtk12.cmake) - working directory is DirectXTK12's own
# fetched source root, per ExternalProject's own documented PATCH_COMMAND
# contract.
#
# DirectXTK12's own vendored CMakeLists.txt runs its shader-compile step
# with a BARE relative script name ("CompileShaders.cmd", no path
# prefix), relying on Windows' implicit current-directory search to find
# it (WORKING_DIRECTORY is set correctly to the folder that script
# actually lives in). That's fine on most machines, but at least one
# machine this project is built on has NoDefaultCurrentDirectoryInExePath
# set (a real Windows security-hardening variable - see Microsoft KB
# Q156276), which disables exactly that implicit search, so the bare
# name can't be found ("no such file or directory", both from cmake -E
# env's own process spawning and from a plain cmd.exe).
#
# There's no reliable way to unset that variable through VS Code's own
# CMake Tools settings either - cmake.environment's documented
# null-to-unset convention gets silently mangled by the extension's own
# environment-expansion helper, which coerces every configured value
# through a string-only macro-expansion call before it ever reaches the
# actual unset-capable merge logic (confirmed by reading the extension's
# own bundled source).
#
# Rewriting the bare name to an absolute path sidesteps the search
# entirely - an absolute path needs no directory search at all, so this
# keeps working regardless of that variable, and regardless of which
# tool (VS Code, real Visual Studio, a plain shell) ends up invoking it.
#
# "CMakeLists.txt", relative - ExternalProject/FetchContent's own
# documented PATCH_COMMAND contract runs this with the working directory
# already set to <SOURCE_DIR>, i.e. exactly where this file lives. This
# script runs in `cmake -P` (script mode) with no project/source-dir
# context of its own, so it must rely on that working directory rather
# than any *_SOURCE_DIR variable.
set(_cmakelists "CMakeLists.txt")

if(NOT EXISTS "${_cmakelists}")
    message(FATAL_ERROR "directxtk12-shader-patch.cmake: could not find ${_cmakelists} in the current working directory - expected to be run as this dependency's own PATCH_COMMAND.")
endif()

file(READ "${_cmakelists}" _contents)

string(REPLACE
    "> CompileShaders.cmd ARGS"
    "> \"\${PROJECT_SOURCE_DIR}/Src/Shaders/CompileShaders.cmd\" ARGS"
    _patched "${_contents}")

if(_patched STREQUAL _contents)
    message(STATUS "directxtk12-shader-patch.cmake: pattern not found - already patched, or upstream changed. Leaving CMakeLists.txt untouched.")
else()
    file(WRITE "${_cmakelists}" "${_patched}")
    message(STATUS "directxtk12-shader-patch.cmake: patched bare CompileShaders.cmd invocation to an absolute path.")
endif()
