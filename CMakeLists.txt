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
# much faster full rebuild. It's a CMake source-grouping feature, so it
# works identically whether the active toolchain is MSVC (cl.exe) or Clang
# (clang-cl) - neither compiler needs to know or care that it's compiling
# batched sources.
#
# It comes at a real cost for tooling, though: CMake only ever writes a
# compile_commands.json entry for whatever a target actually compiles, and
# under a unity build that's the synthesized CMakeFiles/Stand.dir/Unity/
# unity_N_cxx.cxx files - never the original .cpp files. clangd's usual
# trick for a header with no compile command of its own (CommandFoo.hpp,
# say) is to borrow its sibling CommandFoo.cpp's flags; with unity batching
# on, CommandFoo.cpp has no entry to borrow, clangd falls back to a bare
# generic guess, and anything that header needs beyond the standard library
# - <fmt/core.h>, Stand's own headers via STAND_INCLUDE_DIRS, all of it -
# stops resolving.
#
# So this is a cache option (STAND_UNITY_BUILD), not a hardcoded ON:
# leave it ON for a normal full build (the default), or configure with
# -DSTAND_UNITY_BUILD=OFF (already the case for the *-debug presets in
# CMakePresets.json, on the assumption that a Debug configure is usually
# the one clangd is actually pointed at) to get accurate per-file
# compile_commands.json entries instead.
#
# Only the Stand target itself ever reads this (see where it's used near
# add_library(Stand ...)); CMAKE_UNITY_BUILD - the CMake variable that sets
# the *default* UNITY_BUILD property for every target created afterwards -
# is forced OFF right below regardless, so the vendored libraries are
# never batched either way.

option(STAND_UNITY_BUILD
    "Batch multiple .cpp files into fewer translation units for a much faster full rebuild. Turn this OFF for accurate per-file compile_commands.json entries (see the comment above)."
    ON
)

set(CMAKE_UNITY_BUILD OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------
# Directories
# ------------------------------------------------------------

set(STAND_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(LIB_DIR
    "${STAND_DIR}/lib"
)

set(PLUTO_DIR
    "${LIB_DIR}/pluto"
)

set(DISCORD_RPC_DIR
    "${LIB_DIR}/discord-rpc"
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

# StackWalker.cpp (src/lib/stackwalker/) and DirectXTK's curated .cpp
# files (src/lib/directxtk/Src/) are each built into their own target
# further down; without these, both would also get compiled straight into
# Stand via this glob, on top of their own separate target, and linking
# both would collide. fmt/, rapidjson/ and minhook/ don't need an entry
# here: fmt and rapidjson are header-only (nothing under them matches
# *.cpp) and minhook's sources are *.c, which this glob doesn't match
# either.
list(FILTER STAND_CPP EXCLUDE REGEX
    "/stackwalker/"
)

list(FILTER STAND_CPP EXCLUDE REGEX
    "/directxtk/"
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
# soup/ and souplua/ (both under src/lib/, alongside pluto/ and
# discord-rpc/) are excluded too, even though they're part of the Stand
# target's own sources (not compiled separately like pluto/ and
# discord-rpc/ are): a handful of soup/ headers happen to share a bare
# name with an unrelated Stand header (e.g. both have their own
# Worker.hpp), and code that wants a soup/souplua header always spells it
# out qualified - "soup/Foo.hpp" or <soup/Foo.hpp> - resolved via LIB_DIR
# below, never a bare "Foo.hpp" - so adding soup/souplua's own
# subdirectories here would only introduce same-name collisions with zero
# upside.

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
# Vendored under src/lib/directxtk/ instead of being fetched at configure
# time, so it can be patched/improved in place like the rest of src/lib/.
# Vendored from microsoft/DirectXTK @ oct2025
# (273e95b4ac48478f8af1698db9f17cd525f0ede8) - Inc/ (all public headers)
# and Src/'s top-level files (all of upstream's own private headers/
# sources), but only the exact .cpp subset below actually gets compiled.
#
# Effects added to DirectXTK since that subset was chosen (Model*,
# PBREffect*, NormalMapEffect, DebugEffect, NPREffect*) are intentionally
# NOT compiled. Pulling those in would also need freshly compiled HLSL
# shader blobs for them, which requires fxc.exe/dxc.exe - unavailable when
# cross-compiling from Linux (see cmake/cross-compile.cmake), and an
# avoidable extra toolchain dependency even on native Windows. Stand
# doesn't use any of those effects, so it doesn't need their shaders
# compiled at all.
#
# Upstream's own Src/Shaders/ (the HLSL source those blobs are compiled
# from, plus its own compile scripts) isn't vendored at all for the same
# reason: nothing here ever compiles shaders, so there's nothing for it to
# do.
#
# The precompiled shader blobs under src/lib/directxtk/shaders/ (small
# binary build artifacts, not source) cover exactly the effects compiled
# below, so no shader compiler is needed for this project at all, on any
# toolchain. Src/*.cpp files #include these by bare filename (e.g.
# "SpriteEffect_SpriteVertexShader.inc"), so that directory is added to
# DirectXTK's include path directly below - it must stay flat (no
# subdirectories) to match.

set(DIRECTXTK_SOURCE_DIR "${LIB_DIR}/directxtk")

set(DIRECTXTK_CPP_FILES
    AlphaTestEffect.cpp
    BasicEffect.cpp
    BasicPostProcess.cpp
    BinaryReader.cpp
    BufferHelpers.cpp
    CommonStates.cpp
    DDSTextureLoader.cpp
    DGSLEffect.cpp
    DGSLEffectFactory.cpp
    DirectXHelpers.cpp
    DualPostProcess.cpp
    DualTextureEffect.cpp
    EffectCommon.cpp
    EffectFactory.cpp
    EnvironmentMapEffect.cpp
    GamePad.cpp
    GeometricPrimitive.cpp
    Geometry.cpp
    GraphicsMemory.cpp
    Keyboard.cpp
    Mouse.cpp
    PrimitiveBatch.cpp
    ScreenGrab.cpp
    SimpleMath.cpp
    SkinnedEffect.cpp
    SpriteBatch.cpp
    SpriteFont.cpp
    ToneMapPostProcess.cpp
    VertexTypes.cpp
    WICTextureLoader.cpp
)

set(DIRECTXTK_SOURCES "")

foreach(DIRECTXTK_CPP_FILE IN LISTS DIRECTXTK_CPP_FILES)
    set(DIRECTXTK_CPP_PATH "${DIRECTXTK_SOURCE_DIR}/Src/${DIRECTXTK_CPP_FILE}")

    if(NOT EXISTS "${DIRECTXTK_CPP_PATH}")
        message(FATAL_ERROR
            "DirectXTK source file not found: ${DIRECTXTK_CPP_PATH}"
        )
    endif()

    list(APPEND DIRECTXTK_SOURCES "${DIRECTXTK_CPP_PATH}")
endforeach()

add_library(DirectXTK STATIC
    ${DIRECTXTK_SOURCES}
)

target_include_directories(DirectXTK
    PUBLIC
        "${DIRECTXTK_SOURCE_DIR}/Inc"

    PRIVATE
        "${DIRECTXTK_SOURCE_DIR}/Src"

        # Provides the precompiled "*.inc" shader blobs the sources above
        # #include by bare filename.
        "${DIRECTXTK_SOURCE_DIR}/shaders"
)

target_link_libraries(DirectXTK
    PUBLIC
        d3d11
        dxgi
        d3dcompiler
        windowscodecs
)

# ------------------------------------------------------------
# fmt library
# ------------------------------------------------------------
# Vendored under src/lib/fmt/include/fmt/ instead of being fetched at
# configure time, so it can be patched/improved in place like the rest of
# src/lib/. Vendored from fmtlib/fmt @ 9.1.0
# (a33701196adfad74917046096bf5a2aa0ab0bb50) - the same release this
# project has always pinned.
#
# Only the headers are vendored: fmt fully supports a header-only mode
# (its own documented way to use it without compiling anything - see the
# FMT_HEADER_ONLY handling at the bottom of fmt/core.h and fmt/format.h),
# and Stand doesn't call any of the OS-specific functionality format.cc/
# os.cc exist for (fmt::file, fmt::buffered_file, fmt::ostream - nothing
# under src/ references <fmt/os.h> or those types), so there's nothing for
# either .cc file to provide here.

set(FMT_SOURCE_DIR "${LIB_DIR}/fmt")

if(NOT EXISTS "${FMT_SOURCE_DIR}/include/fmt/format.h")
    message(FATAL_ERROR
        "fmt header not found: ${FMT_SOURCE_DIR}/include/fmt/format.h"
    )
endif()

add_library(Fmt INTERFACE)

target_include_directories(Fmt
    INTERFACE
        "${FMT_SOURCE_DIR}/include"
)

target_compile_definitions(Fmt
    INTERFACE
        FMT_HEADER_ONLY
)

# ------------------------------------------------------------
# RapidJSON library
# ------------------------------------------------------------
# Vendored under src/lib/rapidjson/include/ instead of being fetched at
# configure time, so it can be patched/improved in place like the rest of
# src/lib/. Vendored from Tencent/rapidjson @ master
# (24b5e7a8b27f42fa16b96fc70aade9106cf7102f). It's a header-only library -
# nothing under it is a .cpp file - so there's nothing to compile or link
# into a static/shared library; this just exposes its headers through an
# INTERFACE target for Stand's own <rapidjson/...> includes to resolve
# against.

set(RAPIDJSON_SOURCE_DIR "${LIB_DIR}/rapidjson")

if(NOT EXISTS "${RAPIDJSON_SOURCE_DIR}/include/rapidjson/document.h")
    message(FATAL_ERROR
        "RapidJSON header not found: ${RAPIDJSON_SOURCE_DIR}/include/rapidjson/document.h"
    )
endif()

add_library(RapidJSON INTERFACE)

target_include_directories(RapidJSON
    INTERFACE
        "${RAPIDJSON_SOURCE_DIR}/include"
)

# ------------------------------------------------------------
# MinHook library
# ------------------------------------------------------------
# Vendored under src/lib/minhook/ instead of being fetched at configure
# time, so it can be patched/improved in place like the rest of src/lib/.
# Vendored from TsudaKageyu/minhook @ master
# (d94c64d32ea37bc4f5ee47d580709f70c6fb6080).

set(MINHOOK_SOURCE_DIR "${LIB_DIR}/minhook")

set(MINHOOK_SOURCES
    "${MINHOOK_SOURCE_DIR}/src/buffer.c"
    "${MINHOOK_SOURCE_DIR}/src/hook.c"
    "${MINHOOK_SOURCE_DIR}/src/trampoline.c"
    "${MINHOOK_SOURCE_DIR}/src/hde/hde32.c"
    "${MINHOOK_SOURCE_DIR}/src/hde/hde64.c"
)

foreach(MINHOOK_SOURCE_FILE IN LISTS MINHOOK_SOURCES)
    if(NOT EXISTS "${MINHOOK_SOURCE_FILE}")
        message(FATAL_ERROR
            "MinHook source file not found: ${MINHOOK_SOURCE_FILE}"
        )
    endif()
endforeach()

add_library(MinHook STATIC
    ${MINHOOK_SOURCES}
)

target_include_directories(MinHook
    PUBLIC
        "${MINHOOK_SOURCE_DIR}/include"

    PRIVATE
        "${MINHOOK_SOURCE_DIR}/src"
)

# ------------------------------------------------------------
# StackWalker library
# ------------------------------------------------------------
# Vendored under src/lib/stackwalker/ instead of being fetched at
# configure time, so it can be patched/improved in place like the rest of
# src/lib/. Vendored from JochenKalmbach/StackWalker @ master
# (7af402408202a5c00021fd57e18e39e7e6f11062) - only StackWalker.h/.cpp
# (upstream keeps them under Main/StackWalker/, alongside a pile of
# .sln/.vcxproj files for various VS versions and a demo main.cpp, none of
# which is needed here).
#
# Stand's own code includes this library as `<stackwalker/StackWalker.h>`
# (lowercase directory), so the header/source pair is vendored directly
# under a lowercase `stackwalker/` directory rather than mirroring
# upstream's `StackWalker` capitalization - this matters when
# cross-compiling from Linux (see cmake/cross-compile.cmake), where the
# filesystem is case-sensitive.

set(STACKWALKER_SOURCE_DIR "${LIB_DIR}/stackwalker")

if(NOT EXISTS "${STACKWALKER_SOURCE_DIR}/StackWalker.cpp")
    message(FATAL_ERROR
        "StackWalker source not found: ${STACKWALKER_SOURCE_DIR}/StackWalker.cpp"
    )
endif()

add_library(StackWalker STATIC
    "${STACKWALKER_SOURCE_DIR}/StackWalker.cpp"
)

target_include_directories(StackWalker
    PUBLIC
        "${LIB_DIR}"
)

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
        "${LIB_DIR}"
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
        # src/lib/pluto/vendor/Soup/soup/ for its json/regex/crypto/socket/etc.
        # standard-library extensions. It now includes Stand's own soup/
        # library directly (the same one Stand's own code uses, at
        # src/lib/soup/) instead of carrying a second, independently-drifting
        # copy. "${LIB_DIR}" is what makes Pluto's own `#include "soup/..."`
        # references resolve to it.
        "${STAND_DIR}"
        "${LIB_DIR}"
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

        # Stand's code (and Pluto's) reaches the vendored libraries under
        # src/lib/ - soup/, discord-rpc/, souplua/ - via includes like
        # `#include <soup/Rgb.hpp>` or `#include <discord-rpc/include/discord_rpc.h>`,
        # i.e. qualified with the library's own directory name. That only
        # resolves with src/lib/ itself (not each library's own directory)
        # on the include path.
        "${LIB_DIR}"

        # fmt, RapidJSON, MinHook and DirectXTK's own public headers.
        # Linking their targets below (Fmt, RapidJSON, MinHook, DirectXTK)
        # already brings these in transitively through each target's own
        # INTERFACE/PUBLIC target_include_directories() - listed here again
        # directly, the same way Pluto's already are just below, because
        # clangd's fallback for a header with no compile command of its own
        # (most of Stand's .hpp files - see STAND_INCLUDE_DIRS above) isn't
        # always reliable about reconstructing that full transitive chain,
        # and a missing entry here shows up as e.g. `#include <fmt/core.h>`
        # not resolving even though the real build links Fmt just fine.
        "${FMT_SOURCE_DIR}/include"
        "${RAPIDJSON_SOURCE_DIR}/include"
        "${MINHOOK_SOURCE_DIR}/include"
        "${DIRECTXTK_SOURCE_DIR}/Inc"

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