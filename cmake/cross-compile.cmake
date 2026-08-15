# ============================================================
# Stand - Linux -> Windows cross-compilation
# clang-cl + MSVC ABI + msvc-wine
# ============================================================

if(WIN32 OR CYGWIN)
    set(CROSSCOMPILE_DEFAULT OFF)
else()
    set(CROSSCOMPILE_DEFAULT ON)
endif()

option(
    CROSSCOMPILE
    "Enable cross-compilation using clang-cl (MSVC ABI)"
    ${CROSSCOMPILE_DEFAULT}
)

if(NOT CROSSCOMPILE)
    message(STATUS "Cross-compilation is disabled.")
    return()
endif()

message(STATUS "Cross-compile is enabled (clang-cl / MSVC ABI).")

# ------------------------------------------------------------
# Target system
# ------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# ------------------------------------------------------------
# clang-cl
# ------------------------------------------------------------

find_program(CLANG_CL_EXECUTABLE clang-cl)

if(NOT CLANG_CL_EXECUTABLE)
    message(FATAL_ERROR
        "clang-cl not found. Install LLVM."
    )
endif()

set(CMAKE_C_COMPILER
    "${CLANG_CL_EXECUTABLE}"
    CACHE FILEPATH "" FORCE
)

set(CMAKE_CXX_COMPILER
    "${CLANG_CL_EXECUTABLE}"
    CACHE FILEPATH "" FORCE
)

set(CMAKE_C_COMPILER_TARGET
    x86_64-pc-windows-msvc
)

set(CMAKE_CXX_COMPILER_TARGET
    x86_64-pc-windows-msvc
)

# Use LLVM's Windows linker.
find_program(LLD_LINK_EXECUTABLE lld-link)

if(NOT LLD_LINK_EXECUTABLE)
    message(FATAL_ERROR
        "lld-link not found. Install LLVM LLD."
    )
endif()

set(CMAKE_LINKER
    "${LLD_LINK_EXECUTABLE}"
    CACHE FILEPATH "" FORCE
)

message(STATUS "clang-cl: ${CLANG_CL_EXECUTABLE}")
message(STATUS "lld-link: ${LLD_LINK_EXECUTABLE}")

# ------------------------------------------------------------
# MSVC runtime
# ------------------------------------------------------------

set(
    CMAKE_MSVC_RUNTIME_LIBRARY
    MultiThreaded
)

# ------------------------------------------------------------
# msvc-wine
# ------------------------------------------------------------

set(
    MSVC_WINE_ROOT
    "$ENV{HOME}/my_msvc/opt/msvc"
)

if(NOT EXISTS "${MSVC_WINE_ROOT}")
    message(FATAL_ERROR
        "msvc-wine not found at:\n"
        "  ${MSVC_WINE_ROOT}\n"
        "Expected the existing msvc-wine installation."
    )
endif()

message(
    STATUS
    "Using msvc-wine at: ${MSVC_WINE_ROOT}"
)

# ------------------------------------------------------------
# Locate MSVC toolset
# ------------------------------------------------------------

file(
    GLOB MSVC_TOOLSET_DIRS
    "${MSVC_WINE_ROOT}/VC/Tools/MSVC/*"
)

if(NOT MSVC_TOOLSET_DIRS)
    message(FATAL_ERROR
        "MSVC toolset not found under:\n"
        "${MSVC_WINE_ROOT}/VC/Tools/MSVC"
    )
endif()

list(SORT MSVC_TOOLSET_DIRS)
list(GET MSVC_TOOLSET_DIRS -1 MSVC_TOOLSET_DIR)

message(
    STATUS
    "MSVC toolset: ${MSVC_TOOLSET_DIR}"
)

# ------------------------------------------------------------
# Locate Windows SDK
# ------------------------------------------------------------

set(
    WINDOWS_KITS_ROOT
    "${MSVC_WINE_ROOT}/Windows Kits/10"
)

set(CMAKE_ASM_MASM_COMPILER
    "${MSVC_WINE_ROOT}/bin/x64/ml64"
    CACHE FILEPATH ""
    FORCE
)

if(NOT EXISTS "${WINDOWS_KITS_ROOT}")
    message(FATAL_ERROR
        "Windows SDK not found at:\n"
        "${WINDOWS_KITS_ROOT}"
    )
endif()

file(
    GLOB WINDOWS_SDK_VERSIONS
    "${WINDOWS_KITS_ROOT}/Lib/*"
)

if(NOT WINDOWS_SDK_VERSIONS)
    message(FATAL_ERROR
        "Windows SDK library directory not found under:\n"
        "${WINDOWS_KITS_ROOT}/Lib"
    )
endif()

list(SORT WINDOWS_SDK_VERSIONS)
list(GET WINDOWS_SDK_VERSIONS -1 WINDOWS_SDK_VERSION_DIR)

message(
    STATUS
    "Windows SDK: ${WINDOWS_SDK_VERSION_DIR}"
)

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------

set(MSVC_INCLUDE_DIRS
    "${MSVC_TOOLSET_DIR}/include"
)

file(
    GLOB WINDOWS_SDK_INCLUDE_DIRS
    "${WINDOWS_KITS_ROOT}/Include/*/ucrt"
    "${WINDOWS_KITS_ROOT}/Include/*/um"
    "${WINDOWS_KITS_ROOT}/Include/*/shared"
    "${WINDOWS_KITS_ROOT}/Include/*/winrt"
    "${WINDOWS_KITS_ROOT}/Include/*/cppwinrt"
)

list(APPEND
    MSVC_INCLUDE_DIRS
    ${WINDOWS_SDK_INCLUDE_DIRS}
)

foreach(INCLUDE_DIR IN LISTS MSVC_INCLUDE_DIRS)
    if(EXISTS "${INCLUDE_DIR}")
        add_compile_options(
            "/imsvc${INCLUDE_DIR}"
        )
    endif()
endforeach()

# ------------------------------------------------------------
# Library directories
# ------------------------------------------------------------

set(MSVC_LIBRARY_DIRS
    "${MSVC_TOOLSET_DIR}/lib/x64"
)

file(
    GLOB WINDOWS_SDK_LIBRARY_DIRS
    "${WINDOWS_KITS_ROOT}/Lib/*/ucrt/x64"
    "${WINDOWS_KITS_ROOT}/Lib/*/um/x64"
    "${WINDOWS_KITS_ROOT}/Lib/*/onecore/x64"
)

list(APPEND
    MSVC_LIBRARY_DIRS
    ${WINDOWS_SDK_LIBRARY_DIRS}
)

foreach(LIB_DIR IN LISTS MSVC_LIBRARY_DIRS)
    if(EXISTS "${LIB_DIR}")
        add_link_options(
            "/libpath:${LIB_DIR}"
        )
    endif()
endforeach()

# ------------------------------------------------------------
# Environment variables
# ------------------------------------------------------------

string(
    JOIN ":"
    MSVC_LIB_ENV
    ${MSVC_LIBRARY_DIRS}
)

string(
    JOIN ":"
    MSVC_INCLUDE_ENV
    ${MSVC_INCLUDE_DIRS}
)

set(ENV{LIB}
    "${MSVC_LIB_ENV}"
)

set(ENV{INCLUDE}
    "${MSVC_INCLUDE_ENV}"
)

# ------------------------------------------------------------
# MSVC library tool
# ------------------------------------------------------------

find_program(
    MSVC_LIB_EXECUTABLE
    NAMES lib lib.exe
    PATHS
        "${MSVC_WINE_ROOT}/bin/x64"
        "${MSVC_WINE_ROOT}/VC/Tools/MSVC/*/bin/Hostx64/x64"
    NO_DEFAULT_PATH
)

if(MSVC_LIB_EXECUTABLE)
    set(
        CMAKE_AR
        "${MSVC_LIB_EXECUTABLE}"
        CACHE FILEPATH "" FORCE
    )

    set(
        CMAKE_RANLIB
        ":"
        CACHE STRING "" FORCE
    )

    message(
        STATUS
        "MSVC lib.exe: ${MSVC_LIB_EXECUTABLE}"
    )
else()
    message(
        STATUS
        "MSVC lib.exe not found; continuing with lld-link."
    )
endif()

# ------------------------------------------------------------
# Manifest tool
# ------------------------------------------------------------

find_program(
    MSVC_MT_EXECUTABLE
    NAMES mt mt.exe
    PATHS
        "${MSVC_WINE_ROOT}/bin/x64"
    NO_DEFAULT_PATH
)

if(MSVC_MT_EXECUTABLE)
    set(
        CMAKE_MT
        "${MSVC_MT_EXECUTABLE}"
        CACHE FILEPATH "" FORCE
    )

    message(
        STATUS
        "MSVC manifest tool: ${MSVC_MT_EXECUTABLE}"
    )
endif()

# ------------------------------------------------------------
# Linker options
# ------------------------------------------------------------

add_link_options(
    /NOLOGO
    /INCREMENTAL:NO
)

# ------------------------------------------------------------
# Prevent CMake from searching the Linux host for Windows
# libraries/includes.
# ------------------------------------------------------------

set(
    CMAKE_FIND_ROOT_PATH_MODE_PROGRAM
    NEVER
)

set(
    CMAKE_FIND_ROOT_PATH_MODE_LIBRARY
    NEVER
)

set(
    CMAKE_FIND_ROOT_PATH_MODE_INCLUDE
    NEVER
)

set(
    CMAKE_FIND_ROOT_PATH_MODE_PACKAGE
    NEVER
)

message(
    STATUS
    "clang-cl + msvc-wine cross-compilation configured successfully."
)