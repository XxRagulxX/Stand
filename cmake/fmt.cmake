# ============================================================
# Stand - fmt, fetched from fmtlib's GitHub at configure time
# ============================================================
#
# fmt's source used to be committed directly under src/fmt/, including
# src/format.cc and src/os.cc compiled into the build. Neither .cc file is
# actually needed: fmt fully supports a header-only mode (its own documented
# way to use it without compiling anything - see FMT_HEADER_ONLY handling at
# the bottom of fmt/core.h and fmt/format.h), and Stand doesn't call any of
# the OS-specific functionality os.cc exists for (fmt::file,
# fmt::buffered_file, fmt::ostream - nothing under src/ references
# <fmt/os.h> or those types), so there's nothing for either .cc file to
# provide here. This just fetches fmt's headers and lets FMT_HEADER_ONLY
# inline the rest, same as if the .cc files had never been vendored.
#
# Pinned to the exact release that was previously vendored (9.1.0), so
# behavior is unchanged: fmt has made API-visible changes across major
# versions since, and Stand's ~150 call sites of <fmt/...> headers have
# never been checked against anything newer.

include(FetchContent)

set(FMT_GIT_TAG "9.1.0" CACHE STRING "fmt release tag to fetch")

FetchContent_Declare(
    fmt_upstream
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        ${FMT_GIT_TAG}
    GIT_SHALLOW    TRUE

    # fmt has its own top-level CMakeLists.txt, which FetchContent would
    # otherwise add_subdirectory() automatically, creating its own "fmt"
    # target (plus tests/install rules) instead of the plain INTERFACE
    # target defined below. Pointing SOURCE_SUBDIR at include/ (headers
    # only, no CMakeLists.txt of its own) makes FetchContent fetch the
    # source without building it; it does not change where
    # FMT_SOURCE_DIR points below.
    SOURCE_SUBDIR "include"
)

FetchContent_MakeAvailable(fmt_upstream)

set(FMT_SOURCE_DIR "${fmt_upstream_SOURCE_DIR}")

if(NOT EXISTS "${FMT_SOURCE_DIR}/include/fmt/format.h")
    message(FATAL_ERROR
        "fmt header not found after fetch: ${FMT_SOURCE_DIR}/include/fmt/format.h\n"
        "Upstream (tag ${FMT_GIT_TAG}) may have restructured its include layout - "
        "update cmake/fmt.cmake."
    )
endif()

message(STATUS "fmt source: ${FMT_SOURCE_DIR}")

# ------------------------------------------------------------
# fmt library (header-only)
# ------------------------------------------------------------

add_library(Fmt INTERFACE)

target_include_directories(Fmt
    INTERFACE
        "${FMT_SOURCE_DIR}/include"
)

target_compile_definitions(Fmt
    INTERFACE
        FMT_HEADER_ONLY
)
