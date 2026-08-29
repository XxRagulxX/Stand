cmake_minimum_required(VERSION 4.3.4)

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

option(STAND_UNITY_BUILD
    "Batch multiple .cpp files into fewer translation units for a much faster full rebuild, at the cost of per-file compile_commands.json entries (see the comment above) - leave this OFF unless you specifically want a fast from-scratch CI/Release rebuild."
    OFF
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
#
# Every #include of Stand's own code (or of a vendored library under
# src/lib/) is written out fully qualified, relative to src/ - e.g.
# #include "Game/CObject.hpp" or #include "lib/soup/rand.hpp" - so a
# single include root (STAND_DIR, added below on the Stand/Pluto targets)
# is all that's needed to resolve any of them. There is no longer a
# per-directory list to keep in sync as files move between folders, and
# no ambiguity when two folders happen to contain a same-named header
# (see src/README.md).

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
# Vendored from https://github.com/microsoft/DirectXTK, currently at the
# "may2026" release tag (May 7, 2026).

# Defines STAND_COMPILE_DIRECTXTK_SHADERS and, only if it's ON,
# stand_compile_directxtk_shaders() - see cmake/directxtk-shaders.cmake.
include(cmake/directxtk-shaders.cmake)

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

target_compile_options(DirectXTK
    PRIVATE
        /w
)

target_include_directories(DirectXTK
    PUBLIC
        "${DIRECTXTK_SOURCE_DIR}/Inc"

    PRIVATE
        "${DIRECTXTK_SOURCE_DIR}/Src"

        # Provides the compiled "*.inc" shader blobs the sources above
        # #include by bare filename. lib/directxtk/shaders/ holds the
        # *.hlsl these were compiled from (empty unless
        # STAND_COMPILE_DIRECTXTK_SHADERS is being used - see
        # cmake/directxtk-shaders.cmake).
        "${DIRECTXTK_SOURCE_DIR}/shaderscompiled"
)

target_link_libraries(DirectXTK
    PUBLIC
        d3d11
        dxgi
        d3dcompiler
        windowscodecs
)

if(STAND_COMPILE_DIRECTXTK_SHADERS)
    stand_compile_directxtk_shaders(DirectXTK "${DIRECTXTK_SOURCE_DIR}/shaderscompiled")
endif()

# ------------------------------------------------------------
# fmt library
# ------------------------------------------------------------
# Vendored from https://github.com/fmtlib/fmt, currently at the
# "12.2.0" release tag.

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

target_compile_options(MinHook
    PRIVATE
        /w
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

set(STACKWALKER_SOURCE_DIR "${LIB_DIR}/stackwalker")

if(NOT EXISTS "${STACKWALKER_SOURCE_DIR}/StackWalker.cpp")
    message(FATAL_ERROR
        "StackWalker source not found: ${STACKWALKER_SOURCE_DIR}/StackWalker.cpp"
    )
endif()

add_library(StackWalker STATIC
    "${STACKWALKER_SOURCE_DIR}/StackWalker.cpp"
)

target_compile_options(StackWalker
    PRIVATE
        /w
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

target_compile_options(DiscordRPC
    PRIVATE
        /w
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
)

# ------------------------------------------------------------
# Pluto / Lua library
# ------------------------------------------------------------

add_library(Pluto STATIC

    ${PLUTO_C}
    ${PLUTO_CPP}
)

target_compile_options(Pluto
    PRIVATE
        /w
)

target_include_directories(Pluto
    PUBLIC
        "${PLUTO_DIR}"

    PRIVATE
        # Pluto reaches into the vendored Soup library with fully
        # qualified includes, e.g. #include "lib/soup/rand.hpp" - so it
        # only needs src/ itself on the path, same as the Stand target.
        "${STAND_DIR}"
)

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

set_target_properties(Stand PROPERTIES
    UNITY_BUILD ${STAND_UNITY_BUILD}
)

# ------------------------------------------------------------
# Stand include directories
# ------------------------------------------------------------

target_include_directories(Stand
    PRIVATE

        # Every Stand-own header and every vendored library under
        # src/lib/ is reached with a fully qualified #include relative
        # to src/ - e.g. "Game/CObject.hpp" or "lib/soup/rand.hpp" - so
        # src/ itself is the only include root Stand's own code needs.
        "${STAND_DIR}"

        # These vendored libraries are instead reached the way any
        # third-party library normally is - unqualified, by their own
        # public header name (<fmt/core.h>, <rapidjson/document.h>,
        # <MinHook.h>, <lua.h>, DirectXTK's own headers) - so each gets
        # its own public include root here.
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