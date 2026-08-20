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

    # MinHook has its own top-level CMakeLists.txt, which FetchContent would
    # otherwise add_subdirectory() automatically instead of the curated
    # MINHOOK_SOURCES list below. Pointing SOURCE_SUBDIR at include/ (just
    # MinHook.h, no CMakeLists.txt of its own) makes FetchContent fetch the
    # source without building it; it does not change where
    # MINHOOK_SOURCE_DIR points below.
    SOURCE_SUBDIR "include"
)

FetchContent_MakeAvailable(minhook_upstream)

set(MINHOOK_SOURCE_DIR "${minhook_upstream_SOURCE_DIR}")

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
