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

    # StackWalker has its own top-level CMakeLists.txt, which FetchContent
    # would otherwise add_subdirectory() automatically instead of just
    # mirroring the two files below. Pointing SOURCE_SUBDIR at
    # Main/StackWalker (the header/source pair plus some .sln/.vcxproj
    # files, no CMakeLists.txt of its own) makes FetchContent fetch the
    # source without building it; it does not change where
    # stackwalker_upstream_SOURCE_DIR points below.
    SOURCE_SUBDIR "Main/StackWalker"
)

FetchContent_MakeAvailable(stackwalker_upstream)

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
