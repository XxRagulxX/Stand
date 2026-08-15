cmake_minimum_required(VERSION 3.25)

# Required for clang-cl cross-compilation
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(cmake/cross-compile.cmake)

project(Stand LANGUAGES CXX ASM_MASM)

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

set(RAPIDJSON_DIR
    "${STAND_DIR}/rapidjson"
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

# Do not compile embedded third-party libraries as part of Stand.
list(FILTER STAND_CPP EXCLUDE REGEX "/directxtk/")
list(FILTER STAND_CPP EXCLUDE REGEX "/pluto/")
list(FILTER STAND_CPP EXCLUDE REGEX "/fmt/")
list(FILTER STAND_CPP EXCLUDE REGEX "/discord-rpc/")

# ------------------------------------------------------------
# Stand ASM sources
# ------------------------------------------------------------

file(GLOB_RECURSE STAND_ASM CONFIGURE_DEPENDS
    "${STAND_DIR}/*.asm"
)

# ------------------------------------------------------------
# Pluto / Lua
# ------------------------------------------------------------

file(GLOB_RECURSE PLUTO_C CONFIGURE_DEPENDS
    "${PLUTO_DIR}/*.c"
)

file(GLOB_RECURSE PLUTO_CPP CONFIGURE_DEPENDS
    "${PLUTO_DIR}/*.cpp"
)

# ------------------------------------------------------------
# fmt
# ------------------------------------------------------------

file(GLOB FMT_CPP CONFIGURE_DEPENDS
    "${FMT_DIR}/*.cc"
)

# ------------------------------------------------------------
# DirectXTK
# ------------------------------------------------------------

add_library(DirectXTK STATIC

    # Core
    "${DIRECTXTK_DIR}/CommonStates.cpp"
    "${DIRECTXTK_DIR}/DirectXHelpers.cpp"
    "${DIRECTXTK_DIR}/BufferHelpers.cpp"

    # Texture loaders
    "${DIRECTXTK_DIR}/DDSTextureLoader.cpp"
    "${DIRECTXTK_DIR}/WICTextureLoader.cpp"

    # Effects
    "${DIRECTXTK_DIR}/AlphaTestEffect.cpp"
    "${DIRECTXTK_DIR}/BasicEffect.cpp"
    "${DIRECTXTK_DIR}/BasicPostProcess.cpp"
    "${DIRECTXTK_DIR}/DualPostProcess.cpp"
    "${DIRECTXTK_DIR}/DualTextureEffect.cpp"
    "${DIRECTXTK_DIR}/EffectCommon.cpp"
    "${DIRECTXTK_DIR}/EffectFactory.cpp"
    "${DIRECTXTK_DIR}/EnvironmentMapEffect.cpp"
    "${DIRECTXTK_DIR}/SkinnedEffect.cpp"

    # Geometry
    "${DIRECTXTK_DIR}/GeometricPrimitive.cpp"
    "${DIRECTXTK_DIR}/PrimitiveBatch.cpp"
    "${DIRECTXTK_DIR}/VertexTypes.cpp"

    # Sprites / fonts
    "${DIRECTXTK_DIR}/SpriteBatch.cpp"
    "${DIRECTXTK_DIR}/SpriteFont.cpp"

    # Post processing / capture
    "${DIRECTXTK_DIR}/ScreenGrab.cpp"

    # GamePad
    "${DIRECTXTK_DIR}/GamePad.cpp"
)

add_library(DiscordRPC STATIC
    "${DISCORD_RPC_DIR}/src/connection_win.cpp"
    "${DISCORD_RPC_DIR}/src/discord_register_win.cpp"
    "${DISCORD_RPC_DIR}/src/discord_rpc.cpp"
    "${DISCORD_RPC_DIR}/src/rpc_connection.cpp"
    "${DISCORD_RPC_DIR}/src/serialization.cpp"
)

target_include_directories(DirectXTK
    PUBLIC
        "${DIRECTXTK_DIR}"
)

target_include_directories(DiscordRPC
    PUBLIC
        "${DISCORD_RPC_DIR}/include"
    PRIVATE
        "${DISCORD_RPC_DIR}/src"
)

target_link_libraries(DirectXTK
    PUBLIC
        d3d11
        dxgi
        d3dcompiler
        windowscodecs
        DiscordRPC
)

# ------------------------------------------------------------
# Stand target
# ------------------------------------------------------------

add_library(Stand SHARED
    ${STAND_CPP}
    ${STAND_ASM}

    # Pluto / Lua
    ${PLUTO_C}
    ${PLUTO_CPP}

    # fmt
    ${FMT_CPP}
)

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------

target_include_directories(Stand PRIVATE

    # Stand
    "${STAND_DIR}"

    # fmt
    "${FMT_DIR}"

    # RapidJSON
    "${RAPIDJSON_DIR}"

    # Pluto / Lua
    "${PLUTO_DIR}"

    # DirectXTK
    "${DIRECTXTK_DIR}"
)

# ------------------------------------------------------------
# Precompiled header
# ------------------------------------------------------------

target_precompile_headers(Stand PRIVATE
    "${STAND_DIR}/common.hpp"
)

# ------------------------------------------------------------
# Libraries
# ------------------------------------------------------------

target_link_libraries(Stand PRIVATE
    DirectXTK
)

# ------------------------------------------------------------
# Compiler options
# ------------------------------------------------------------

if(MSVC)
    target_compile_options(Stand PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
        $<$<COMPILE_LANGUAGE:CXX>:/permissive->
        $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
        $<$<COMPILE_LANGUAGE:CXX>:/utf-8>
        $<$<COMPILE_LANGUAGE:CXX>:/bigobj>
    )
endif()