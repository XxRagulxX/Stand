# ============================================================
# Stand - RapidJSON, fetched from Tencent's GitHub at configure time
# ============================================================
#
# RapidJSON's source used to be committed directly under src/rapidjson/.
# It's a header-only library - nothing under it is a .cpp file - so there's
# nothing to compile or link into a static/shared library; this just fetches
# its headers via FetchContent and exposes them through an INTERFACE target
# for Stand's own <rapidjson/...> includes to resolve against.
#
# Pinned to the exact release that was previously vendored (1.1.0, per
# src/rapidjson/rapidjson.h's old RAPIDJSON_MAJOR/MINOR/PATCH_VERSION
# defines), so behavior is unchanged.

include(FetchContent)

set(RAPIDJSON_GIT_TAG "master" CACHE STRING "RapidJSON branch to fetch")

FetchContent_Declare(
    rapidjson_upstream
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG        ${RAPIDJSON_GIT_TAG}
    GIT_SHALLOW    TRUE

    # RapidJSON has its own top-level CMakeLists.txt (builds its test suite,
    # docs, etc.), which FetchContent would otherwise add_subdirectory()
    # automatically instead of the plain INTERFACE target defined below.
    # Pointing SOURCE_SUBDIR at include/ (headers only, no CMakeLists.txt of
    # its own) makes FetchContent fetch the source without building it; it
    # does not change where RAPIDJSON_SOURCE_DIR points below.
    SOURCE_SUBDIR "include"
)

FetchContent_MakeAvailable(rapidjson_upstream)

set(RAPIDJSON_SOURCE_DIR "${rapidjson_upstream_SOURCE_DIR}")

if(NOT EXISTS "${RAPIDJSON_SOURCE_DIR}/include/rapidjson/document.h")
    message(FATAL_ERROR
        "RapidJSON header not found after fetch: ${RAPIDJSON_SOURCE_DIR}/include/rapidjson/document.h\n"
        "Upstream (tag ${RAPIDJSON_GIT_TAG}) may have restructured its include layout - "
        "update cmake/rapidjson.cmake."
    )
endif()

message(STATUS "RapidJSON source: ${RAPIDJSON_SOURCE_DIR}")

# ------------------------------------------------------------
# RapidJSON library (header-only)
# ------------------------------------------------------------

add_library(RapidJSON INTERFACE)

target_include_directories(RapidJSON
    INTERFACE
        "${RAPIDJSON_SOURCE_DIR}/include"
)
