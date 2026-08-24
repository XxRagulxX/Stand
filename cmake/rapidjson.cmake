# ============================================================
# Stand - RapidJSON, vendored under src/lib/rapidjson/
# ============================================================
#
# RapidJSON is vendored directly (src/lib/rapidjson/include/) instead of
# being fetched at configure time, so it can be patched/improved in place
# like the rest of src/lib/. Vendored from Tencent/rapidjson @ master
# (24b5e7a8b27f42fa16b96fc70aade9106cf7102f). It's a header-only library -
# nothing under it is a .cpp file - so there's nothing to compile or link
# into a static/shared library; this just exposes its headers through an
# INTERFACE target for Stand's own <rapidjson/...> includes to resolve
# against.

set(RAPIDJSON_SOURCE_DIR "${LIB_DIR}/rapidjson")

if(NOT EXISTS "${RAPIDJSON_SOURCE_DIR}/include/rapidjson/document.h")
    message(FATAL_ERROR
        "RapidJSON header not found: ${RAPIDJSON_SOURCE_DIR}/include/rapidjson/document.h"
    )
endif()

# ------------------------------------------------------------
# RapidJSON library (header-only)
# ------------------------------------------------------------

add_library(RapidJSON INTERFACE)

target_include_directories(RapidJSON
    INTERFACE
        "${RAPIDJSON_SOURCE_DIR}/include"
)
