# ============================================================
# Stand - MinHook, fetched from GitHub at configure time
# ============================================================
#
# MinHook's source is no longer vendored under src/. Instead, this file
# fetches it via FetchContent and compiles it straight from the fetched
# tree. Unlike DirectXTK, there's no curation needed here: MinHook is small,
# has no generated/shader-compiler dependency, and its file layout has been
# stable for years - so all of its sources are compiled as-is.

include(FetchContent)

set(MINHOOK_GIT_TAG "master" CACHE STRING "MinHook branch/tag to fetch")

FetchContent_Declare(
    minhook_upstream
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        ${MINHOOK_GIT_TAG}
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(minhook_upstream)
if(NOT minhook_upstream_POPULATED)
    message(STATUS "Fetching MinHook (${MINHOOK_GIT_TAG}) from https://github.com/TsudaKageyu/minhook ...")
    FetchContent_Populate(minhook_upstream)
endif()

set(MINHOOK_SOURCE_DIR "${minhook_upstream_SOURCE_DIR}")

message(STATUS "MinHook source: ${MINHOOK_SOURCE_DIR}")

set(MINHOOK_SOURCES
    "${MINHOOK_SOURCE_DIR}/src/buffer.c"
    "${MINHOOK_SOURCE_DIR}/src/hook.c"
    "${MINHOOK_SOURCE_DIR}/src/trampoline.c"
    "${MINHOOK_SOURCE_DIR}/src/hde/hde32.c"
    "${MINHOOK_SOURCE_DIR}/src/hde/hde64.c"
)

foreach(MINHOOK_SOURCE_FILE IN LISTS MINHOOK_SOURCES)
    if(NOT EXISTS "${MINHOOK_SOURCE_FILE}")
        message(FATAL_ERROR
            "MinHook source file not found after fetch: ${MINHOOK_SOURCE_FILE}\n"
            "Upstream (tag ${MINHOOK_GIT_TAG}) may have moved/renamed this file - "
            "update cmake/minhook.cmake's MINHOOK_SOURCES list."
        )
    endif()
endforeach()

# ------------------------------------------------------------
# MinHook library
# ------------------------------------------------------------

add_library(MinHook STATIC
    ${MINHOOK_SOURCES}
)

target_include_directories(MinHook
    PUBLIC
        "${MINHOOK_SOURCE_DIR}/include"

    PRIVATE
        "${MINHOOK_SOURCE_DIR}/src"
)
