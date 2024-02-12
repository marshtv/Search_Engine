#
# Created by marsh on 30.01.2024.
#

cmake_minimum_required(VERSION 3.27)
project(Search_Engine)

set(CMAKE_CXX_STANDARD 23)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        json
        URL
        https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

enable_testing()

# target_link_libraries(Search_Engine PRIVATE nlohmann_json::nlohmann_json)
# target_link_libraries(Search_Engine PRIVATE gtest_main)
include(GoogleTest)
# gtest_discover_tests(Search_Engine)

# add_subdirectory(resources)
add_subdirectory(src)
add_subdirectory(tests)