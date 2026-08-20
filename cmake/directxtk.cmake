# ============================================================
# Stand - DirectXTK, fetched from Microsoft's GitHub at configure time
# ============================================================
#
# DirectXTK's full upstream source is no longer vendored under src/. Instead,
# this file fetches it via FetchContent and compiles only the .cpp files
# Stand actually uses - the same set that used to be committed directly to
# this repository, just sourced from upstream instead of copy-pasted here.
#
# Effects added to DirectXTK since that vendored copy was taken (Model*,
# PBREffect*, NormalMapEffect, DebugEffect, NPREffect*) are intentionally NOT
# compiled. Pulling those in would also need freshly compiled HLSL shader
# blobs for them, which requires fxc.exe/dxc.exe - unavailable when
# cross-compiling from Linux (see cmake/cross-compile.cmake), and an
# avoidable extra toolchain dependency even on native Windows. Stand doesn't
# use any of those effects, so it doesn't need their shaders compiled at all.
#
# The precompiled shader blobs under directxtk-shaders/ (checked into this
# repo - they're small binary build artifacts, not source) cover exactly
# the effects compiled below, so no shader compiler is needed for this
# project at all, on any toolchain. Upstream's Src/*.cpp files #include
# these by bare filename (e.g. "SpriteEffect_SpriteVertexShader.inc"), so
# directxtk-shaders/ is added to DirectXTK's include path directly below -
# it must stay flat (no subdirectories) to match.

include(FetchContent)

set(DIRECTXTK_GIT_TAG "oct2025" CACHE STRING "DirectXTK release tag to fetch")

# FetchContent_Declare() + FetchContent_GetProperties() + a bare
# FetchContent_Populate(<name>) is the classic "fetch source but don't
# add_subdirectory() it" recipe, but that specific form of Populate() (the
# one that reuses details from a prior Declare()) is deprecated as of CMake
# 3.30 (policy CMP0169) in favor of FetchContent_MakeAvailable() - which
# doesn't fit here, since MakeAvailable() would run DirectXTK's own
# CMakeLists.txt via add_subdirectory(), which is exactly what this file
# exists to avoid (see the notes above). CMake's own docs still document
# and support giving FetchContent_Populate() all the content details
# directly, with no prior Declare() - that form isn't deprecated, and is
# used below instead.
message(STATUS "Fetching DirectXTK (${DIRECTXTK_GIT_TAG}) from https://github.com/microsoft/DirectXTK ...")

FetchContent_Populate(
    directxtk_upstream
    GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
    GIT_TAG        ${DIRECTXTK_GIT_TAG}
    GIT_SHALLOW    TRUE
    SOURCE_DIR     "${CMAKE_BINARY_DIR}/_deps/directxtk_upstream-src"
    SUBBUILD_DIR   "${CMAKE_BINARY_DIR}/_deps/directxtk_upstream-subbuild"
    BINARY_DIR     "${CMAKE_BINARY_DIR}/_deps/directxtk_upstream-build"
)

set(DIRECTXTK_SOURCE_DIR "${directxtk_upstream_SOURCE_DIR}")

message(STATUS "DirectXTK source: ${DIRECTXTK_SOURCE_DIR}")

# ------------------------------------------------------------
# The exact subset of upstream Src/*.cpp files Stand builds.
# ------------------------------------------------------------

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
            "DirectXTK source file not found after fetch: ${DIRECTXTK_CPP_PATH}\n"
            "Upstream (tag ${DIRECTXTK_GIT_TAG}) may have moved/renamed this file - "
            "update cmake/directxtk.cmake's DIRECTXTK_CPP_FILES list."
        )
    endif()

    list(APPEND DIRECTXTK_SOURCES "${DIRECTXTK_CPP_PATH}")
endforeach()

# ------------------------------------------------------------
# DirectXTK library
# ------------------------------------------------------------

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
        "${CMAKE_CURRENT_SOURCE_DIR}/directxtk-shaders"
)

target_link_libraries(DirectXTK
    PUBLIC
        d3d11
        dxgi
        d3dcompiler
        windowscodecs
)
