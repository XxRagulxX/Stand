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

# Populated via FetchContent_Populate()'s direct-arguments form (content
# details passed straight to Populate(), no prior Declare()) rather than the
# classic Declare()+GetProperties()+Populate(<name>) recipe - the latter's
# bare Populate(<name>) call is deprecated as of CMake 3.30 (policy
# CMP0169) in favor of FetchContent_MakeAvailable(), which doesn't apply
# here since it would add_subdirectory() MinHook's own CMakeLists.txt. The
# direct-arguments form used below isn't deprecated - see
# cmake/directxtk.cmake for the fuller explanation (same reasoning applies
# here).
message(STATUS "Fetching MinHook (${MINHOOK_GIT_TAG}) from https://github.com/TsudaKageyu/minhook ...")

FetchContent_Populate(
    minhook_upstream
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        ${MINHOOK_GIT_TAG}
    GIT_SHALLOW    TRUE
    SOURCE_DIR     "${CMAKE_BINARY_DIR}/_deps/minhook_upstream-src"
    SUBBUILD_DIR   "${CMAKE_BINARY_DIR}/_deps/minhook_upstream-subbuild"
    BINARY_DIR     "${CMAKE_BINARY_DIR}/_deps/minhook_upstream-build"
)

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
