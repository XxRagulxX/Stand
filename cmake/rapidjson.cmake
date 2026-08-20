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
)

FetchContent_GetProperties(rapidjson_upstream)
if(NOT rapidjson_upstream_POPULATED)
    message(STATUS "Fetching RapidJSON (${RAPIDJSON_GIT_TAG}) from https://github.com/Tencent/rapidjson ...")
    FetchContent_Populate(rapidjson_upstream)
endif()

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
