# ============================================================
# Cross-compile dispatcher
# ============================================================

if(WIN32 OR CYGWIN)
    message(STATUS "Loading Windows cross-compile toolchain")
    include(${CMAKE_CURRENT_LIST_DIR}/cross-compile-windows.cmake)
else()
    message(STATUS "Loading Linux cross-compile toolchain")
    include(${CMAKE_CURRENT_LIST_DIR}/cross-compile-linux.cmake)
endif()
