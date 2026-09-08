cmake_minimum_required(VERSION 3.20)

# REQUIRED for clang-cl cross-compilation (Windows target on Linux)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(cmake/cross-compile.cmake)

project(
    StandEnhanced
    VERSION 0.0.1
    DESCRIPTION "A new base using new C++ features optimised for speed and ease of use"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Libraries

include(cmake/async-logger.cmake)
include(cmake/json.cmake)
include(cmake/minhook.cmake)
include(cmake/luajit.cmake)
include(cmake/directxtk12.cmake)

# Source

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${SRC_DIR}/*.hpp"
    "${SRC_DIR}/*.cpp"
)

# StandEnhanced target

add_library(${PROJECT_NAME} MODULE
    ${SRC_FILES}
)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY CXX_STANDARD 23
)

# MASM / x64 assembly

if(MSVC)
    enable_language(ASM_MASM)

    set(EXCEPTION_ASM_FILES
        "${SRC_DIR}/Core/exceptional_helper.asm"
        "${SRC_DIR}/Core/get_stack_pointer.asm"
    )

    set_source_files_properties(
        ${EXCEPTION_ASM_FILES}
        PROPERTIES
            LANGUAGE ASM_MASM
    )

    target_sources(
        ${PROJECT_NAME}
        PRIVATE
            ${EXCEPTION_ASM_FILES}
    )
endif()

# Precompiled headers

target_precompile_headers(
    ${PROJECT_NAME}
    PUBLIC
        "${SRC_DIR}/Core/common.hpp"
)

# Compile definitions

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

# fmt

if(TARGET fmt AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
        fmt
        PRIVATE
            -Wno-deprecated-literal-operator
    )
endif()

# LuaJIT

if(TARGET luajit)
    target_compile_definitions(
        luajit
        PRIVATE
            _CRT_SECURE_NO_WARNINGS
    )
endif()

# Include directories

message(STATUS "Setting up include directories.")

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        "${SRC_DIR}"
        "${minhook_SOURCE_DIR}/include"
        "${minhook_SOURCE_DIR}/src/hde"
        "${luajit_SOURCE_DIR}/src"
        "${luajit_BINARY_DIR}/src"
)


# Libraries

message(STATUS "Setting up linked libraries")

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE

        # Project dependencies
        AsyncLogger
        minhook
        nlohmann_json::nlohmann_json
        DirectXTK12
        dbghelp

        # Windows/D3D system libraries (originally pulled in alongside
        # ImGui's own DX12/Win32 backends - kept even after ImGui's
        # removal since DirectXTK12/the game's own window handling may
        # still depend on some of these; not worth the risk of pruning
        # blind with no compiler available to verify against)
        d3dcompiler
        dwmapi
        gdi32
        dxgi
        libluajit

        # DirectXTK12 dependency: EffectDeviceResources::DemandCreateRootSig
        # calls D3D12SerializeRootSignature, which lives in d3d12.lib. Nothing
        # else in this project links it directly (the D3D12 device/queue are
        # obtained from the game's existing swapchain, not D3D12CreateDevice).
        d3d12
)

# Compiler options

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE

        # Do not warn about non-reproducible code
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-date-time>
        $<$<COMPILE_LANG_AND_ID:CXX,Clang>:-Wno-date-time>
)