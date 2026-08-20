# ============================================================
# Stand - Native Windows build
# MSVC (cl.exe) or Clang (clang-cl.exe), x64 only
# Debug and Release
# ============================================================
#
# This file is included right after project() (before any add_library()
# calls) so that the runtime-library selection below applies to every
# target in the build. It is split into two parts:
#
#   1. stand_apply_common_compile_options() - flags that apply to the Stand
#      target on EVERY build of this project, native or cross-compiled,
#      because both use an MSVC-compatible compiler driver. Always
#      available, and also called by CMakeLists.txt for the Linux -> Windows
#      cross build (see cmake/cross-compile.cmake).
#
#   2. Everything else - native-Windows-only setup (x64 enforcement,
#      compiler identification, MSVC runtime library, /MP), which returns
#      immediately when cross-compiling so it never conflicts with
#      cross-compile.cmake's toolchain.

function(stand_apply_common_compile_options target)
    target_compile_options(${target}
        PRIVATE
            $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
            $<$<COMPILE_LANGUAGE:CXX>:/permissive->
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
            $<$<COMPILE_LANGUAGE:CXX>:/utf-8>
            $<$<COMPILE_LANGUAGE:CXX>:/bigobj>

            # Stand's PCH (common.hpp) pulls in soup/ObfusString.hpp, whose
            # rand.hpp seeds an obfuscation PRNG from __TIME__ - deliberately,
            # so string literals wrapped in ObfusString get a different
            # compile-time seed per build. Clang warns that baking
            # __DATE__/__TIME__ into a PCH means every translation unit
            # sharing that PCH sees the same frozen value until the PCH
            # itself is regenerated; true, and an accepted trade-off for the
            # compile-time win a PCH gives here, not something to fix by
            # unpicking ObfusString.hpp back out of common.hpp.
            $<$<CXX_COMPILER_ID:Clang>:-Wno-pch-date-time>
    )
endfunction()

# ------------------------------------------------------------
# Everything below is for a native (non cross-compiled) Windows build only.
# ------------------------------------------------------------

if(CROSSCOMPILE OR NOT WIN32)
    return()
endif()

message(STATUS "Native Windows build detected.")

# ------------------------------------------------------------
# Enforce x64
# ------------------------------------------------------------
# Stand only supports 64-bit (the game itself is x64-only), so fail here
# with a clear message instead of much later with confusing linker errors.

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR
        "Stand must be built for x64, but the active toolset/platform is "
        "not 64-bit (CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P}).\n"
        "If you're using Visual Studio's CMake integration (CMakeSettings.json), "
        "make sure the active configuration inherits an x64 environment "
        "(e.g. \"msvc_x64_x64\" or \"clang_cl_x64_x64\").\n"
        "If you're invoking CMake directly with the Visual Studio generator, "
        "pass -A x64."
    )
endif()

# ------------------------------------------------------------
# Identify the compiler
# ------------------------------------------------------------
# Stand only supports real MSVC (cl.exe) or Clang's MSVC-compatible driver
# (clang-cl.exe) - both share the same command-line syntax and ABI, which
# this codebase relies on (__try/__except, __declspec, etc.). Plain
# clang++/g++ (GNU-style driver) are not supported natively.

set(STAND_USING_MSVC FALSE)
set(STAND_USING_CLANG_CL FALSE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(STAND_USING_MSVC TRUE)
    message(STATUS "Native toolchain: MSVC (cl.exe)")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    set(STAND_USING_CLANG_CL TRUE)
    message(STATUS "Native toolchain: Clang (clang-cl.exe), MSVC ABI")
else()
    message(FATAL_ERROR
        "Unsupported compiler for a native Windows build: "
        "${CMAKE_CXX_COMPILER_ID} (frontend variant: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}).\n"
        "Stand must be built with either MSVC (cl.exe) or Clang's "
        "MSVC-compatible driver (clang-cl.exe)."
    )
endif()

# ------------------------------------------------------------
# MSVC runtime library
# ------------------------------------------------------------
# Static CRT (matching the Linux cross-compile toolchain, and required so
# Stand and its statically-linked dependencies - DirectXTK, MinHook, Pluto,
# DiscordRPC - all agree on one CRT), with the Debug/Release variant picked
# automatically per configuration. Must be set before any target is created,
# which is why this file is included immediately after project().

set(CMAKE_MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# ------------------------------------------------------------
# Per-target compile options
# ------------------------------------------------------------
# Called later, once the Stand target exists.

function(stand_apply_native_windows_options target)
    stand_apply_common_compile_options(${target})

    if(STAND_USING_MSVC)
        # Lets cl.exe compile multiple translation units per invocation.
        # Only meaningful for the Visual Studio (MSBuild) generator; harmless
        # (and unnecessary) with Ninja, where parallelism comes from -j.
        # clang-cl doesn't support/need this, so it's MSVC-only.
        target_compile_options(${target}
            PRIVATE
                $<$<COMPILE_LANGUAGE:CXX>:/MP>
        )
    endif()
endfunction()
