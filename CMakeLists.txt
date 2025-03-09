set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.14)
project(bybit-cli)

include(FetchContent)

# Glaze, JSON
FetchContent_Declare(
    glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG main
)
FetchContent_MakeAvailable(glaze)

# CLI parser
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2 
)
FetchContent_MakeAvailable(CLI11)

# HTTP Client
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06) 
FetchContent_MakeAvailable(cpr)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bybit-cli src/main.cpp )


target_include_directories(bybit-cli PRIVATE ${glaze_SOURCE_DIR}/include)
target_link_libraries(bybit-cli PRIVATE CLI11::CLI11)
target_link_libraries(bybit-cli PRIVATE cpr::cpr)