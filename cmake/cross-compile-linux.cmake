# ============================================================
# Cross-compilation settings (clang-cl / MSVC ABI via msvc-wine)
# ============================================================

if(WIN32 OR CYGWIN)
    set(CROSSCOMPILE_DEFAULT OFF)
else()
    set(CROSSCOMPILE_DEFAULT ON)
endif()

option(CROSSCOMPILE "Enable cross-compilation using clang-cl (MSVC ABI)" ${CROSSCOMPILE_DEFAULT})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CROSSCOMPILE)
    message(STATUS "Cross-compile is enabled (clang-cl / MSVC ABI).")

    # ------------------------------------------------------------
    # Add msvc-wine bin to PATH so bare 'ml64' resolves at build time
    # (CMake's ASM_MASM language detection caches the bare name; the
    # wrapper scripts that call wine live only in this directory).
    # ------------------------------------------------------------
    set(MSVC_WINE_BIN "$ENV{HOME}/my_msvc/opt/msvc/bin/x64")
    set(ENV{PATH} "${MSVC_WINE_BIN}:$ENV{PATH}")

    # ------------------------------------------------------------
    # Target system
    # ------------------------------------------------------------
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)

    # ------------------------------------------------------------
    # Locate clang-cl
    # ------------------------------------------------------------
    find_program(CLANG_CL_EXECUTABLE clang-cl)
    if(NOT CLANG_CL_EXECUTABLE)
        message(FATAL_ERROR "clang-cl not found. Install LLVM.")
    endif()

    set(CMAKE_C_COMPILER ${CLANG_CL_EXECUTABLE} CACHE STRING "" FORCE)
    set(CMAKE_CXX_COMPILER ${CLANG_CL_EXECUTABLE} CACHE STRING "" FORCE)
    set(CMAKE_LINKER lld-link CACHE STRING "" FORCE)

    set(CMAKE_C_COMPILER_TARGET x86_64-pc-windows-msvc)
    set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-windows-msvc)

    # ------------------------------------------------------------
    # MSVC runtime
    # ------------------------------------------------------------
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)

    # ------------------------------------------------------------
    # msvc-wine root (CHANGE IF NEEDED)
    # ------------------------------------------------------------
    set(MSVC_WINE_ROOT "$ENV{HOME}/my_msvc/opt/msvc")

    if(NOT EXISTS "${MSVC_WINE_ROOT}/bin")
        message(FATAL_ERROR
            "msvc-wine not found at ${MSVC_WINE_ROOT}\n"
            "Install it using https://github.com/mstorsjo/msvc-wine"
        )
    endif()

    message(STATUS "Using msvc-wine at: ${MSVC_WINE_ROOT}")

    # ------------------------------------------------------------
    # Locate MSVC tools
    # ------------------------------------------------------------
    find_program(MSVC_LIB_EXECUTABLE
        NAMES lib lib.exe
        PATHS "${MSVC_WINE_ROOT}/bin/x64"
        NO_DEFAULT_PATH
    )

    if(NOT MSVC_LIB_EXECUTABLE)
        message(FATAL_ERROR "MSVC lib.exe not found!")
    endif()

    set(CMAKE_AR "${MSVC_LIB_EXECUTABLE}" CACHE FILEPATH "" FORCE)
    set(CMAKE_RANLIB ":" CACHE STRING "" FORCE)

    # ------------------------------------------------------------
    # Inject MSVC + Windows SDK paths
    # ------------------------------------------------------------
    file(GLOB MSVC_TOOLSET_DIR "${MSVC_WINE_ROOT}/VC/Tools/MSVC/*")
    file(GLOB WINDOWS_SDK_DIR "${MSVC_WINE_ROOT}/Windows Kits/10")

    list(SORT MSVC_TOOLSET_DIR)
    list(GET MSVC_TOOLSET_DIR -1 MSVC_TOOLSET_DIR)
    list(SORT WINDOWS_SDK_DIR)
    list(GET WINDOWS_SDK_DIR -1 WINDOWS_SDK_DIR)

    file(GLOB MSVC_LIB_DIR "${MSVC_TOOLSET_DIR}/lib/x64")
    file(GLOB WINDOWS_SDK_LIB_DIR
        "${WINDOWS_SDK_DIR}/Lib/*/ucrt/x64"
        "${WINDOWS_SDK_DIR}/Lib/*/um/x64"
        "${WINDOWS_SDK_DIR}/Lib/*/onecore/x64"
    )

    file(GLOB MSVC_INC_DIR "${MSVC_TOOLSET_DIR}/include")
    file(GLOB WINDOWS_SDK_INC_DIR
        "${WINDOWS_SDK_DIR}/Include/*/ucrt"
        "${WINDOWS_SDK_DIR}/Include/*/um"
        "${WINDOWS_SDK_DIR}/Include/*/shared"
        "${WINDOWS_SDK_DIR}/Include/*/winrt"
        "${WINDOWS_SDK_DIR}/Include/*/cppwinrt"
    )

    list(APPEND MSVC_LIB_DIR ${WINDOWS_SDK_LIB_DIR})
    list(APPEND MSVC_INC_DIR ${WINDOWS_SDK_INC_DIR})

    set(MSVC_LIB_PATHS ${MSVC_LIB_DIR})
    set(MSVC_INC_PATHS ${MSVC_INC_DIR})

    string(JOIN ":" MSVC_LIB_ENV ${MSVC_LIB_PATHS})
    string(JOIN ":" MSVC_INC_ENV ${MSVC_INC_PATHS})

    foreach(MSVC_INCLUDE_PATH IN LISTS MSVC_INC_DIR)
        add_compile_options("/imsvc${MSVC_INCLUDE_PATH}")
    endforeach()

    foreach(MSVC_LIBRARY_PATH IN LISTS MSVC_LIB_DIR)
        add_link_options("/libpath:${MSVC_LIBRARY_PATH}")
    endforeach()

    set(ENV{LIB} "${MSVC_LIB_ENV}")
    set(ENV{INCLUDE} "${MSVC_INC_ENV}")
    # ------------------------------------------------------------
    # MSVC Manifest Tool
    # ------------------------------------------------------------
    find_program(MSVC_MT_EXECUTABLE
        NAMES mt mt.exe
        PATHS "${MSVC_WINE_ROOT}/bin/x64"
        NO_DEFAULT_PATH
    )

    if(NOT MSVC_MT_EXECUTABLE)
        message(FATAL_ERROR "MSVC mt.exe not found!")
    endif()

    set(CMAKE_MT "${MSVC_MT_EXECUTABLE}" CACHE FILEPATH "" FORCE)

    # ------------------------------------------------------------
    # MASM assembler (ml64) - used when enable_language(ASM_MASM)
    # is called later in CMakeLists.txt.  Without an explicit full
    # path cmake stores the bare name "ml64", which is not on PATH
    # at build time (only the msvc-wine bin dir has it).
    # ------------------------------------------------------------
    # Use the wrapper script that rewrites relative -Fo paths to absolute
    # paths before calling ml64 via wine (wine's CWD differs from the
    # shell's CWD, so relative output paths write to the wrong place).
    set(_ml64_wrapper "${MSVC_WINE_ROOT}/bin/x64/ml64-wrapper")
    if(EXISTS "${_ml64_wrapper}")
        set(CMAKE_ASM_MASM_COMPILER "${_ml64_wrapper}" CACHE FILEPATH "" FORCE)
    else()
        find_program(MSVC_ML64_EXECUTABLE
            NAMES ml64 ml64.exe
            PATHS "${MSVC_WINE_ROOT}/bin/x64"
            NO_DEFAULT_PATH
        )
        if(MSVC_ML64_EXECUTABLE)
            set(CMAKE_ASM_MASM_COMPILER "${MSVC_ML64_EXECUTABLE}" CACHE FILEPATH "" FORCE)
        else()
            message(WARNING "ml64 not found in ${MSVC_WINE_ROOT}/bin/x64 — MASM (.asm) files will not assemble.")
        endif()
    endif()

    # ------------------------------------------------------------
    # Disable MinGW / Unix search behavior
    # ------------------------------------------------------------
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)

    # ------------------------------------------------------------
    # Compiler flags (safe for DLL injection)
    # CXX-only flags use generator expressions so they are NOT passed
    # to the MASM assembler — ml64 does not understand /Zc:__cplusplus
    # and silently drops its output when it appears on the command line.
    # ------------------------------------------------------------
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
        $<$<COMPILE_LANGUAGE:CXX>:/permissive->
        $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
        $<$<COMPILE_LANGUAGE:CXX>:/Zc:inline>
        $<$<COMPILE_LANGUAGE:CXX>:/GS->
    )

    # ------------------------------------------------------------
    # Linker flags
    # ------------------------------------------------------------
    add_link_options(
        /NOLOGO
        /INCREMENTAL:NO
    )

    message(STATUS "clang-cl + msvc-wine toolchain configured successfully.")
endif()
