include(FetchContent)

set(LIB_NAME "directxtk12")
message(STATUS "Setting up ${LIB_NAME}")

# DirectXTK12 is used as the drawing primitives layer for the new
# Stand-style custom renderer (src/Rendering/). It's DirectX12-native,
# unlike DirectXTK (D3D11), which is what stand-reference itself uses.
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_FUZZING OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_XAUDIO_WIN10 OFF CACHE BOOL "" FORCE)
set(BUILD_XAUDIO_REDIST OFF CACHE BOOL "" FORCE)
set(BUILD_GAMEINPUT OFF CACHE BOOL "" FORCE)
set(BUILD_WGI OFF CACHE BOOL "" FORCE)
set(BUILD_XINPUT OFF CACHE BOOL "" FORCE)

# On Linux, CompileShaders.cmd is a Windows batch script that cannot run
# natively. We compile the shaders ourselves via wine+dxc.exe at cmake
# configure time and point DirectXTK12 at the pre-built results.
# This block must run BEFORE FetchContent_MakeAvailable so the cache
# variables are visible inside DirectXTK12's own CMakeLists.txt.
# NOTE: WIN32 is TRUE even on Linux when cross-compiling to Windows
# (it reflects the TARGET system, not the build host). Use
# CMAKE_HOST_WIN32 to detect the actual build machine OS.
if(NOT CMAKE_HOST_WIN32)
    set(_dxtk12_shaders_dir "${CMAKE_BINARY_DIR}/dxtk12-compiled-shaders")
    file(MAKE_DIRECTORY "${_dxtk12_shaders_dir}")
    set(USE_PREBUILT_SHADERS ON CACHE BOOL "" FORCE)
    set(COMPILED_SHADERS "${_dxtk12_shaders_dir}" CACHE PATH "" FORCE)
endif()

# DirectXTK12's own CMakeLists.txt invokes its shader-compile step
# ("Generating HLSL shaders...") via a bare relative "CompileShaders.cmd",
# which needs Windows' implicit current-directory search to resolve -
# disabled on any machine with NoDefaultCurrentDirectoryInExePath set (a
# real security-hardening variable, see Microsoft KB Q156276), which
# fails the build with a bare "no such file or directory" and no other
# indication why. See directxtk12-shader-patch.cmake's own comment for
# why this is patched here rather than worked around through environment
# variables.
FetchContent_Declare(
    ${LIB_NAME}
    GIT_REPOSITORY https://github.com/microsoft/DirectXTK12.git
    GIT_TAG        may2026
    GIT_PROGRESS TRUE
    PATCH_COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/directxtk12-shader-patch.cmake"
)
FetchContent_MakeAvailable(${LIB_NAME})

# After FetchContent (so directxtk12_SOURCE_DIR is set): on Linux, compile
# the HLSL shaders via wine+dxc.exe at configure time if not already done.
# This runs once; subsequent configures (and builds) reuse the .inc files.
if(NOT CMAKE_HOST_WIN32)
    set(_dxc_exe "$ENV{HOME}/my_msvc/opt/msvc/Windows Kits/10/Redist/D3D/x64/dxc.exe")
    set(_shader_src "${directxtk12_SOURCE_DIR}/Src/Shaders")
    set(_canary "${_dxtk12_shaders_dir}/SpriteEffect_SpriteVertexShader.inc")

    if(NOT EXISTS "${_canary}")
        if(NOT EXISTS "${_dxc_exe}")
            message(FATAL_ERROR
                "dxc.exe not found at ${_dxc_exe}\n"
                "Expected msvc-wine to be installed at ~/my_msvc/opt/msvc.\n"
                "See cmake/cross-compile-linux.cmake for setup instructions.")
        endif()

        find_program(_wine_exe wine)
        if(NOT _wine_exe)
            message(FATAL_ERROR
                "wine not found in PATH. Install wine to compile DirectXTK12 shaders.\n"
                "  sudo apt-get install wine")
        endif()

        message(STATUS "Compiling DirectXTK12 HLSL shaders via wine+dxc.exe (one-time step)...")

        # wine maps z: to the filesystem root (/), so Linux absolute paths
        # become z:\abs\path inside the Windows environment.
        string(REPLACE "/" "\\" _win_output   "z:${_dxtk12_shaders_dir}")
        string(REPLACE "/" "\\" _win_dxc      "z:${_dxc_exe}")
        string(REPLACE "/" "\\" _win_script   "z:${_shader_src}/CompileShaders.cmd")

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E env
                "CompileShadersOutput=${_win_output}"
                "DirectXShaderCompiler=${_win_dxc}"
                "WINEDEBUG=-all"
                "WINEARCH=win64"
                "${_wine_exe}" cmd.exe /c "${_win_script}" dxil
            WORKING_DIRECTORY "${_shader_src}"
            OUTPUT_FILE "${_dxtk12_shaders_dir}/compileshaders.log"
            ERROR_FILE  "${_dxtk12_shaders_dir}/compileshaders.err"
            RESULT_VARIABLE _shader_result
        )

        if(NOT EXISTS "${_canary}")
            message(FATAL_ERROR
                "DirectXTK12 shader compilation via wine failed (exit ${_shader_result}).\n"
                "Check the logs:\n"
                "  ${_dxtk12_shaders_dir}/compileshaders.log\n"
                "  ${_dxtk12_shaders_dir}/compileshaders.err")
        endif()

        message(STATUS "DirectXTK12 HLSL shaders compiled successfully.")
    endif()
endif()
