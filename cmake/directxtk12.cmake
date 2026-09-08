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
