include(FetchContent)

message("LuaJIT")

# For some reason putting this into crosscompile.cmake doesn't work.
if(CROSSCOMPILE)
    set(CMAKE_SYSTEM_VERSION 10.0.26100)
endif()
set(LUAJIT_BUILD_TOOL OFF)
set(LUAJIT_FORCE_UTF8_FOPEN ON)

FetchContent_Declare(
    luajit
    GIT_REPOSITORY https://github.com/WohlSoft/LuaJIT.git
    GIT_TAG 41594f160375b25e6df19e6130c602d98d5b9c24
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(luajit)
target_compile_definitions(libluajit PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "LUAJIT_ENABLE_LUA52COMPAT"
)