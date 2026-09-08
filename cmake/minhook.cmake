include(FetchContent)

set(LIB_NAME "minhook")
message(STATUS "Setting up ${LIB_NAME}")

FetchContent_Declare(
    ${LIB_NAME}
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        1e9ad1eb42db11bfcb65461f687c656612d1b555
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(${LIB_NAME})