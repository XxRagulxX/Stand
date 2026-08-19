# ============================================================
# Stand - fmt, fetched from fmtlib's GitHub at configure time
# ============================================================
#
# fmt's source used to be committed directly under src/fmt/. Instead, this
# file fetches it via FetchContent and compiles the two .cc files fmt itself
# needs (src/format.cc, src/os.cc) - the same pair that used to live at
# src/fmt/format.cc and src/fmt/os.cc, just sourced from upstream instead of
# copy-pasted here.
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
)

FetchContent_GetProperties(fmt_upstream)
if(NOT fmt_upstream_POPULATED)
    message(STATUS "Fetching fmt (${FMT_GIT_TAG}) from https://github.com/fmtlib/fmt ...")
    FetchContent_Populate(fmt_upstream)
endif()

set(FMT_SOURCE_DIR "${fmt_upstream_SOURCE_DIR}")

message(STATUS "fmt source: ${FMT_SOURCE_DIR}")

# ------------------------------------------------------------
# fmt library
# ------------------------------------------------------------

set(FMT_CPP_FILES
    format.cc
    os.cc
)

set(FMT_SOURCES "")

foreach(FMT_CPP_FILE IN LISTS FMT_CPP_FILES)
    set(FMT_CPP_PATH "${FMT_SOURCE_DIR}/src/${FMT_CPP_FILE}")

    if(NOT EXISTS "${FMT_CPP_PATH}")
        message(FATAL_ERROR
            "fmt source file not found after fetch: ${FMT_CPP_PATH}\n"
            "Upstream (tag ${FMT_GIT_TAG}) may have moved/renamed this file - "
            "update cmake/fmt.cmake's FMT_CPP_FILES list."
        )
    endif()

    list(APPEND FMT_SOURCES "${FMT_CPP_PATH}")
endforeach()

add_library(Fmt STATIC
    ${FMT_SOURCES}
)

target_include_directories(Fmt
    PUBLIC
        "${FMT_SOURCE_DIR}/include"
)
