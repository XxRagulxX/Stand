# ============================================================
# Stand - StackWalker, fetched from GitHub at configure time
# ============================================================
#
# StackWalker's source is no longer vendored under src/. Instead, this file
# fetches it via FetchContent - it doesn't need its own library target,
# just its header/source made available for the Stand target to compile
# directly, same as it was when it lived under src/stackwalker/.
#
# Stand's own code includes this as `<stackwalker/StackWalker.h>`
# (lowercase directory), but upstream keeps the header/source pair under
# `Main/StackWalker` (capitalized, and nested inside a "Main" folder that's
# just how that repo is laid out - not part of anything Stand depends on).
# Rather than touch every #include across the codebase, the header/source
# pair is mirrored into a lowercase `stackwalker/` directory in the build
# tree so the include resolves exactly like it used to - including on
# case-sensitive filesystems, which matters when cross-compiling from Linux
# (see cmake/cross-compile.cmake).
#
# This file defines two variables for CMakeLists.txt to consume:
#   STACKWALKER_INCLUDE_DIR - add to the Stand target's include directories
#   STACKWALKER_SOURCES     - add to the Stand target's source list

include(FetchContent)

set(STACKWALKER_GIT_TAG "master" CACHE STRING "StackWalker branch/tag to fetch")

# Populated via FetchContent_Populate()'s direct-arguments form (content
# details passed straight to Populate(), no prior Declare()) rather than the
# classic Declare()+GetProperties()+Populate(<name>) recipe - the latter's
# bare Populate(<name>) call is deprecated as of CMake 3.30 (policy
# CMP0169) in favor of FetchContent_MakeAvailable(), which doesn't apply
# here anyway since only two files get copied out below, not the whole
# repo added as a subdirectory. The direct-arguments form used below isn't
# deprecated - see cmake/directxtk.cmake for the fuller explanation.
message(STATUS "Fetching StackWalker (${STACKWALKER_GIT_TAG}) from https://github.com/JochenKalmbach/StackWalker ...")

FetchContent_Populate(
    stackwalker_upstream
    GIT_REPOSITORY https://github.com/JochenKalmbach/StackWalker.git
    GIT_TAG        ${STACKWALKER_GIT_TAG}
    GIT_SHALLOW    TRUE
    SOURCE_DIR     "${CMAKE_BINARY_DIR}/_deps/stackwalker_upstream-src"
    SUBBUILD_DIR   "${CMAKE_BINARY_DIR}/_deps/stackwalker_upstream-subbuild"
    BINARY_DIR     "${CMAKE_BINARY_DIR}/_deps/stackwalker_upstream-build"
)

set(STACKWALKER_UPSTREAM_DIR "${stackwalker_upstream_SOURCE_DIR}/Main/StackWalker")

if(NOT EXISTS "${STACKWALKER_UPSTREAM_DIR}/StackWalker.cpp")
    message(FATAL_ERROR
        "StackWalker source not found after fetch: ${STACKWALKER_UPSTREAM_DIR}/StackWalker.cpp\n"
        "Upstream (tag ${STACKWALKER_GIT_TAG}) may have moved this file - "
        "update cmake/stackwalker.cmake."
    )
endif()

# ------------------------------------------------------------
# Mirror StackWalker.h/.cpp into a lowercase `stackwalker/` directory so
# `#include <stackwalker/StackWalker.h>` keeps resolving unchanged.
# ------------------------------------------------------------

set(STACKWALKER_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/stackwalker-include")

file(MAKE_DIRECTORY "${STACKWALKER_INCLUDE_DIR}/stackwalker")

file(COPY
        "${STACKWALKER_UPSTREAM_DIR}/StackWalker.h"
        "${STACKWALKER_UPSTREAM_DIR}/StackWalker.cpp"
    DESTINATION
        "${STACKWALKER_INCLUDE_DIR}/stackwalker"
)

set(STACKWALKER_SOURCES
    "${STACKWALKER_INCLUDE_DIR}/stackwalker/StackWalker.cpp"
)

# StackWalker.cpp switches on dbghelp's SymType enum without a case for
# NumSymTypes (a sentinel/count value, not a real symbol type - dbghelp
# itself never returns it). That's upstream's code, not ours to edit here
# since it's re-fetched/re-copied on every configure, so the -Wswitch this
# produces under Clang is silenced for just this one file instead. Scoped to
# this file rather than the whole Stand target so it doesn't hide a real
# missing-case bug in Stand's own switches.
set_source_files_properties(${STACKWALKER_SOURCES}
    PROPERTIES COMPILE_OPTIONS
        "$<$<CXX_COMPILER_ID:Clang>:-Wno-switch>"
)
