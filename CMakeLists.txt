cmake_minimum_required(VERSION 3.14)
project(bybit-cli)

include(FetchContent)
FetchContent_Declare(
    glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG main
)
FetchContent_MakeAvailable(glaze)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bybit-cli main.cpp)
target_include_directories(bybit-cli PRIVATE ${glaze_SOURCE_DIR}/include)

