cmake_minimum_required(VERSION 3.25)

# Required for clang-cl cross-compilation
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(cmake/cross-compile.cmake)

project(Stand LANGUAGES C CXX ASM_MASM)

# ------------------------------------------------------------
# C / C++ configuration
# ------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_UNITY_BUILD OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------
# Directories
# ------------------------------------------------------------

set(STAND_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(DIRECTXTK_DIR
    "${STAND_DIR}/directxtk"
)

set(PLUTO_DIR
    "${STAND_DIR}/pluto"
)

set(FMT_DIR
    "${STAND_DIR}/fmt"
)

set(DISCORD_RPC_DIR
    "${STAND_DIR}/discord-rpc"
)

set(MINHOOK_DIR
    "${STAND_DIR}/minhook"
)

# ------------------------------------------------------------
# Stand C++ sources
# ------------------------------------------------------------

file(GLOB_RECURSE STAND_CPP CONFIGURE_DEPENDS
    "${STAND_DIR}/*.cpp"
)

# Third-party libraries are compiled separately.
list(FILTER STAND_CPP EXCLUDE REGEX
    "/directxtk/"
)

list(FILTER STAND_CPP EXCLUDE REGEX
    "/pluto/"
)

list(FILTER STAND_CPP EXCLUDE REGEX
    "/fmt/"
)

list(FILTER STAND_CPP EXCLUDE REGEX
    "/discord-rpc/"
)

list(FILTER STAND_CPP EXCLUDE REGEX
    "/minhook/"
)

# ------------------------------------------------------------
# Stand ASM sources
# ------------------------------------------------------------

file(GLOB_RECURSE STAND_ASM CONFIGURE_DEPENDS
    "${STAND_DIR}/*.asm"
)

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
# fmt sources
# ------------------------------------------------------------

file(GLOB_RECURSE FMT_CPP CONFIGURE_DEPENDS
    "${FMT_DIR}/*.cc"
)

# ------------------------------------------------------------
# DirectXTK sources
# ------------------------------------------------------------

file(GLOB_RECURSE DIRECTXTK_CPP CONFIGURE_DEPENDS
    "${DIRECTXTK_DIR}/*.cpp"
)

# ------------------------------------------------------------
# MinHook sources
# ------------------------------------------------------------

file(GLOB MINHOOK_C CONFIGURE_DEPENDS
    "${MINHOOK_DIR}/src/*.c"
    "${MINHOOK_DIR}/src/hde/*.c"
)

# ------------------------------------------------------------
# DirectXTK library
# ------------------------------------------------------------

add_library(DirectXTK STATIC
    ${DIRECTXTK_CPP}
)

target_include_directories(DirectXTK
    PUBLIC
        "${DIRECTXTK_DIR}"
)

target_link_libraries(DirectXTK
    PUBLIC
        d3d11
        dxgi
        d3dcompiler
        windowscodecs
)

# ------------------------------------------------------------
# MinHook library
# ------------------------------------------------------------

add_library(MinHook STATIC
    ${MINHOOK_C}
)

target_include_directories(MinHook
    PUBLIC
        "${MINHOOK_DIR}/include"

    PRIVATE
        "${MINHOOK_DIR}/src"
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

target_include_directories(DiscordRPC
    PUBLIC
        "${DISCORD_RPC_DIR}/include"

    PRIVATE
        "${DISCORD_RPC_DIR}/src"
        "${STAND_DIR}"
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
)

# IMPORTANT:
# Pluto has its own vendored Soup implementation under:
#
#     src/pluto/vendor/Soup/soup/
#
# Therefore Pluto must not inherit Stand's PCH.
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

    # fmt
    ${FMT_CPP}
)

# ------------------------------------------------------------
# Stand include directories
# ------------------------------------------------------------

target_include_directories(Stand
    PRIVATE

        # Stand
        "${STAND_DIR}"

        # fmt
        "${FMT_DIR}"

        # Pluto / Lua public headers
        "${PLUTO_DIR}"

        # DirectXTK
        "${DIRECTXTK_DIR}"

        # Stackwalker
        "${STAND_DIR}/stackwalker"
)

# ------------------------------------------------------------
# Stand precompiled header
# ------------------------------------------------------------

target_precompile_headers(Stand
    PRIVATE
        "${STAND_DIR}/common.hpp"
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

        # Discord RPC
        DiscordRPC

        # MinHook
        MinHook

        dbghelp
)

# ------------------------------------------------------------
# Compiler options
# ------------------------------------------------------------

if(MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(Stand
        PRIVATE

            $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
            $<$<COMPILE_LANGUAGE:CXX>:/permissive->
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
            $<$<COMPILE_LANGUAGE:CXX>:/utf-8>
            $<$<COMPILE_LANGUAGE:CXX>:/bigobj>
    )
endif()