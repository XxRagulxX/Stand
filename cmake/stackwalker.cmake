# ============================================================
# Stand - StackWalker, vendored under src/lib/stackwalker/
# ============================================================
#
# StackWalker is vendored directly (src/lib/stackwalker/) instead of being
# fetched at configure time, so it can be patched/improved in place like
# the rest of src/lib/. Vendored from JochenKalmbach/StackWalker @ master
# (7af402408202a5c00021fd57e18e39e7e6f11062) - only StackWalker.h/.cpp
# (upstream keeps them under Main/StackWalker/, alongside a pile of
# .sln/.vcxproj files for various VS versions and a demo main.cpp, none of
# which is needed here).
#
# Stand's own code includes this library as `<stackwalker/StackWalker.h>`
# (lowercase directory), so the header/source pair is vendored directly
# under a lowercase `stackwalker/` directory rather than mirroring
# upstream's `StackWalker` capitalization - this matters when
# cross-compiling from Linux (see cmake/cross-compile.cmake), where the
# filesystem is case-sensitive.

set(STACKWALKER_SOURCE_DIR "${LIB_DIR}/stackwalker")

if(NOT EXISTS "${STACKWALKER_SOURCE_DIR}/StackWalker.cpp")
    message(FATAL_ERROR
        "StackWalker source not found: ${STACKWALKER_SOURCE_DIR}/StackWalker.cpp"
    )
endif()

# ------------------------------------------------------------
# StackWalker library
# ------------------------------------------------------------

add_library(StackWalker STATIC
    "${STACKWALKER_SOURCE_DIR}/StackWalker.cpp"
)

target_include_directories(StackWalker
    PUBLIC
        "${LIB_DIR}"
)
