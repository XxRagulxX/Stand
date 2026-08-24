# ============================================================
# Stand - fmt, vendored under src/lib/fmt/
# ============================================================
#
# fmt is vendored directly (src/lib/fmt/include/fmt/) instead of being
# fetched at configure time, so it can be patched/improved in place like
# the rest of src/lib/. Vendored from fmtlib/fmt @ 9.1.0
# (a33701196adfad74917046096bf5a2aa0ab0bb50) - the same release this
# project has always pinned.
#
# Only the headers are vendored: fmt fully supports a header-only mode
# (its own documented way to use it without compiling anything - see the
# FMT_HEADER_ONLY handling at the bottom of fmt/core.h and fmt/format.h),
# and Stand doesn't call any of the OS-specific functionality format.cc/
# os.cc exist for (fmt::file, fmt::buffered_file, fmt::ostream - nothing
# under src/ references <fmt/os.h> or those types), so there's nothing
# for either .cc file to provide here.

set(FMT_SOURCE_DIR "${LIB_DIR}/fmt")

if(NOT EXISTS "${FMT_SOURCE_DIR}/include/fmt/format.h")
    message(FATAL_ERROR
        "fmt header not found: ${FMT_SOURCE_DIR}/include/fmt/format.h"
    )
endif()

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
