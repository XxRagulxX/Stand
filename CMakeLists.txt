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
# Discord RPC sources
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
# Stand target
# ------------------------------------------------------------

add_library(Stand SHARED

    # Stand C++
    ${STAND_CPP}

    # Stand ASM
    ${STAND_ASM}

    # Pluto / Lua
    ${PLUTO_C}
    ${PLUTO_CPP}

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

        # Pluto / Lua
        "${PLUTO_DIR}"

        # DirectXTK
        "${DIRECTXTK_DIR}"
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

        # DirectXTK
        DirectXTK

        # Discord RPC
        DiscordRPC
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