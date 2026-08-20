# ============================================================
# Stand - StackWalker, fetched from GitHub at configure time
# ============================================================
#
# StackWalker's source is no longer vendored under src/. Instead, this file
# fetches it via FetchContent and builds it as its own static library, the
# same way cmake/directxtk.cmake and cmake/minhook.cmake do for those
# dependencies.
#
# Stand's own code includes this library as `<stackwalker/StackWalker.h>`
# (lowercase directory), but upstream keeps the header/source pair under
# `Main/StackWalker` (capitalized, and nested inside a "Main" folder that's
# just how that repo is laid out - not part of anything Stand depends on).
# Rather than touch every #include across the codebase, the header/source
# pair is mirrored into a lowercase `stackwalker/` directory in the build
# tree so the include resolves exactly like it used to - including on
# case-sensitive filesystems, which matters when cross-compiling from Linux
# (see cmake/cross-compile.cmake).

include(FetchContent)

set(STACKWALKER_GIT_TAG "master" CACHE STRING "StackWalker branch/tag to fetch")

FetchContent_Declare(
    stackwalker_upstream
    GIT_REPOSITORY https://github.com/JochenKalmbach/StackWalker.git
    GIT_TAG        ${STACKWALKER_GIT_TAG}
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(stackwalker_upstream)
if(NOT stackwalker_upstream_POPULATED)
    message(STATUS "Fetching StackWalker (${STACKWALKER_GIT_TAG}) from https://github.com/JochenKalmbach/StackWalker ...")
    FetchContent_Populate(stackwalker_upstream)
endif()

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

set(STACKWALKER_INCLUDE_ROOT "${CMAKE_BINARY_DIR}/_deps/stackwalker-include")

file(MAKE_DIRECTORY "${STACKWALKER_INCLUDE_ROOT}/stackwalker")

file(COPY
        "${STACKWALKER_UPSTREAM_DIR}/StackWalker.h"
        "${STACKWALKER_UPSTREAM_DIR}/StackWalker.cpp"
    DESTINATION
        "${STACKWALKER_INCLUDE_ROOT}/stackwalker"
)

# ------------------------------------------------------------
# StackWalker library
# ------------------------------------------------------------

add_library(StackWalker STATIC
    "${STACKWALKER_INCLUDE_ROOT}/stackwalker/StackWalker.cpp"
)

target_include_directories(StackWalker
    PUBLIC
        "${STACKWALKER_INCLUDE_ROOT}"
)
