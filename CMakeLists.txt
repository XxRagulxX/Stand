cmake_minimum_required(VERSION 3.25)

# Required for clang-cl cross-compilation
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(cmake/cross-compile.cmake)

project(Stand LANGUAGES C CXX ASM_MASM)

# Native Windows build (MSVC or clang-cl, x64, Debug/Release). No-op when
# cmake/cross-compile.cmake above is already handling the build (Linux ->
# Windows cross-compilation). Included this early so its MSVC runtime
# library selection applies before any target below is created.
include(cmake/windows.cmake)

# ------------------------------------------------------------
# C / C++ configuration
# ------------------------------------------------------------

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Unity build batches multiple .cpp files into fewer translation units for a
# much faster full rebuild. It's controlled by the single switch right
# below: set CMAKE_UNITY_BUILD to ON to enable it, OFF to disable it.
#
# That value is captured into STAND_UNITY_BUILD immediately, then
# CMAKE_UNITY_BUILD itself is forced back OFF - CMake uses CMAKE_UNITY_BUILD
# as the default UNITY_BUILD property for every target created afterwards,
# and leaving it ON here would also batch the vendored libraries below.
# STAND_UNITY_BUILD is what actually gets applied, and only to the Stand
# target itself, further down - see where it's used near add_library(Stand
# ...). This is purely a CMake source-grouping feature, so it works
# identically whether the active toolchain is MSVC (cl.exe) or Clang
# (clang-cl) - neither compiler needs to know or care that it's compiling
# batched sources.

set(CMAKE_UNITY_BUILD ON)

set(STAND_UNITY_BUILD ${CMAKE_UNITY_BUILD})

set(CMAKE_UNITY_BUILD OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------
# Directories
# ------------------------------------------------------------

set(STAND_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(PLUTO_DIR
    "${STAND_DIR}/pluto"
)

set(DISCORD_RPC_DIR
    "${STAND_DIR}/discord-rpc"
)

# ------------------------------------------------------------
# Stand C++ sources
# ------------------------------------------------------------

file(GLOB_RECURSE STAND_CPP CONFIGURE_DEPENDS
    "${STAND_DIR}/*.cpp"
)

# Third-party libraries are compiled separately.
list(FILTER STAND_CPP EXCLUDE REGEX
    "/pluto/"
)

list(FILTER STAND_CPP EXCLUDE REGEX
    "/discord-rpc/"
)

# ------------------------------------------------------------
# Stand ASM sources
# ------------------------------------------------------------

file(GLOB_RECURSE STAND_ASM CONFIGURE_DEPENDS
    "${STAND_DIR}/*.asm"
)

# ------------------------------------------------------------
# Stand include directories
# ------------------------------------------------------------
# Stand's source tree is organized into subfolders by area (Core, Game,
# Network, Commands/Vehicle, Commands/Self, etc. - see src/ for the full
# layout), but its files still #include each other by bare filename, e.g.
# `#include "Renderer.hpp"` rather than `#include "Rendering/Renderer.hpp"`.
# That only resolves if every directory containing a header is on the
# include search path, so this collects them all, one entry per directory
# that actually has a .hpp in it. Vendored libraries manage their own
# include directories elsewhere in this file, so they're excluded here.
#
# soup/ and souplua/ are excluded too, even though they're part of the
# Stand target's own sources (not compiled separately like pluto/ and
# discord-rpc/): a handful of soup/ headers happen to share a bare name
# with an unrelated Stand header (e.g. both have their own Worker.hpp),
# and code outside soup/ that actually wants a soup header always spells
# it "soup/Foo.hpp" (resolved via STAND_DIR below), never a bare "Foo.hpp"
# - so adding soup/souplua's own subdirectories here would only introduce
# same-name collisions with zero upside.

file(GLOB_RECURSE STAND_HEADERS CONFIGURE_DEPENDS
    "${STAND_DIR}/*.hpp"
)

list(FILTER STAND_HEADERS EXCLUDE REGEX
    "/pluto/"
)

list(FILTER STAND_HEADERS EXCLUDE REGEX
    "/discord-rpc/"
)

list(FILTER STAND_HEADERS EXCLUDE REGEX
    "/soup/"
)

list(FILTER STAND_HEADERS EXCLUDE REGEX
    "/souplua/"
)

set(STAND_INCLUDE_DIRS "")

foreach(stand_header ${STAND_HEADERS})
    get_filename_component(stand_header_dir "${stand_header}" DIRECTORY)
    list(APPEND STAND_INCLUDE_DIRS "${stand_header_dir}")
endforeach()

list(REMOVE_DUPLICATES STAND_INCLUDE_DIRS)

# ------------------------------------------------------------
# Pluto / Lua sources
# ------------------------------------------------------------

file(GLOB_RECURSE PLUTO_C CONFIGURE_DEPENDS
    "${PLUTO_DIR}/*.c"
)

file(GLOB_RECURSE PLUTO_CPP CONFIGURE_DEPENDS
    "${PLUTO_DIR}/*.cpp"
)

# ------------------------------------------------------------
# DirectXTK library
# ------------------------------------------------------------
# Fetched from GitHub and built from a curated file list; see
# cmake/directxtk.cmake. Defines the DirectXTK target linked below.

include(cmake/directxtk.cmake)

# ------------------------------------------------------------
# fmt library
# ------------------------------------------------------------
# Fetched from GitHub and built from its two .cc files; see cmake/fmt.cmake.
# Defines the Fmt target linked below.

include(cmake/fmt.cmake)

# ------------------------------------------------------------
# RapidJSON library
# ------------------------------------------------------------
# Fetched from GitHub (header-only, nothing to compile); see
# cmake/rapidjson.cmake. Defines the RapidJSON target linked below.

include(cmake/rapidjson.cmake)

# ------------------------------------------------------------
# MinHook library
# ------------------------------------------------------------
# Fetched from GitHub (master); see cmake/minhook.cmake. Defines the
# MinHook target linked below.

include(cmake/minhook.cmake)

# ------------------------------------------------------------
# StackWalker library
# ------------------------------------------------------------
# Fetched from GitHub (master); see cmake/stackwalker.cmake. Defines the
# StackWalker target linked below.

include(cmake/stackwalker.cmake)

# ------------------------------------------------------------
# Discord RPC library
# ------------------------------------------------------------

add_library(DiscordRPC STATIC

    "${DISCORD_RPC_DIR}/src/connection_win.cpp"
    "${DISCORD_RPC_DIR}/src/discord_register_win.cpp"
    "${DISCORD_RPC_DIR}/src/discord_rpc.cpp"
    "${DISCORD_RPC_DIR}/src/rpc_connection.cpp"
    "${DISCORD_RPC_DIR}/src/serialization.cpp"
)

target_link_libraries(DiscordRPC
    PRIVATE
        # serialization.h (private header, not part of DiscordRPC's public
        # API) uses rapidjson/document.h.
        RapidJSON
)

target_include_directories(DiscordRPC
    PUBLIC
        "${DISCORD_RPC_DIR}/include"

    PRIVATE
        "${DISCORD_RPC_DIR}/src"
        "${STAND_DIR}"
        ${STAND_INCLUDE_DIRS}
)

# ------------------------------------------------------------
# Pluto / Lua library
# ------------------------------------------------------------

add_library(Pluto STATIC

    ${PLUTO_C}
    ${PLUTO_CPP}
)

target_include_directories(Pluto
    PUBLIC
        "${PLUTO_DIR}"

    PRIVATE
        # Pluto used to vendor its own copy of Soup under
        # src/pluto/vendor/Soup/soup/ for its json/regex/crypto/socket/etc.
        # standard-library extensions. It now includes Stand's own soup/
        # library directly (the same one Stand's own code uses) instead of
        # carrying a second, independently-drifting copy.
        "${STAND_DIR}"
        ${STAND_INCLUDE_DIRS}
)

# IMPORTANT:
# Pluto must not inherit Stand's PCH (common.hpp): that PCH pulls in
# Stand's GTA-specific/Command headers (CommandPhysical.hpp, natives.hpp,
# etc.), which have nothing to do with a general-purpose Lua/Pluto library
# and risk colliding with Pluto's own internal macros - e.g. the
# `#undef G` / `#undef isLua` in CommandLuaScript.cpp exist precisely
# because Lua's internals and Stand's own code can't coexist under one set
# of macros.
#
# Stand's PCH is attached only to the Stand target below,
# so no SKIP_PRECOMPILE_HEADERS is required here.

# ------------------------------------------------------------
# Stand target
# ------------------------------------------------------------

add_library(Stand SHARED

    # Stand C++
    ${STAND_CPP}

    # Stand ASM
    ${STAND_ASM}
)

# ------------------------------------------------------------
# Stand unity build
# ------------------------------------------------------------
# Only Stand's own sources (STAND_CPP) get batched. Fmt is header-only
# (an INTERFACE target with nothing to compile) and DirectXTK, RapidJSON,
# MinHook and StackWalker are each their own separate library target - none
# of that vendored code is part of the Stand target's source list above, so
# none of it is touched by Stand's UNITY_BUILD property in the first place.
# A unity-build name collision surfacing in code we don't own would mean
# patching it, which is exactly what's been avoided everywhere else in this
# build; keeping the vendored libraries on their own targets, each compiled
# as ordinary (non-unity) translation units, sidesteps that entirely rather
# than requiring per-file SKIP_UNITY_BUILD_INCLUSION opt-outs.

set_target_properties(Stand PROPERTIES
    UNITY_BUILD ${STAND_UNITY_BUILD}
)

# ------------------------------------------------------------
# Stand include directories
# ------------------------------------------------------------

target_include_directories(Stand
    PRIVATE

        # Stand - one include directory per subfolder that has a header in
        # it (see STAND_INCLUDE_DIRS above), since Stand's own #includes are
        # bare filenames with no subfolder prefix.
        "${STAND_DIR}"
        ${STAND_INCLUDE_DIRS}

        # Pluto / Lua public headers
        "${PLUTO_DIR}"
)

# ------------------------------------------------------------
# Stand precompiled header
# ------------------------------------------------------------

target_precompile_headers(Stand
    PRIVATE
        "${STAND_DIR}/Core/common.hpp"
)

# ------------------------------------------------------------
# Stand libraries
# ------------------------------------------------------------

target_link_libraries(Stand
    PRIVATE

        # Pluto / Lua
        Pluto

        # DirectXTK
        DirectXTK

        # fmt
        Fmt

        # RapidJSON
        RapidJSON

        # Discord RPC
        DiscordRPC

        # MinHook
        MinHook

        # StackWalker
        StackWalker

        dbghelp
)

# ------------------------------------------------------------
# Compiler options
# ------------------------------------------------------------
# See cmake/windows.cmake (native MSVC/clang-cl build) and
# cmake/cross-compile.cmake (Linux -> Windows cross build) for where these
# functions come from.

if(CROSSCOMPILE)
    stand_apply_common_compile_options(Stand)
else()
    stand_apply_native_windows_options(Stand)
endif()