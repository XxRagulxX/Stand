# ============================================================
# Stand - MinHook, vendored under src/lib/minhook/
# ============================================================
#
# MinHook is vendored directly (src/lib/minhook/) instead of being fetched
# at configure time, so it can be patched/improved in place like the rest
# of src/lib/. Vendored from TsudaKageyu/minhook @ master
# (d94c64d32ea37bc4f5ee47d580709f70c6fb6080).

set(MINHOOK_SOURCE_DIR "${LIB_DIR}/minhook")

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
            "MinHook source file not found: ${MINHOOK_SOURCE_FILE}"
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
