cmake_minimum_required(VERSION 3.27)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SrcFiles
        ../converterjson.h
        ../converterjson.cpp
        ../invertedIndex.h
        ../invertedIndex.cpp
        ../searchServer.h
        ../searchServer.cpp

)

set(TestsFiles
        search_engine_test.cc
)

add_executable(GtestTests ${TestsFiles} ${SrcFiles})
target_link_libraries(GtestTests nlohmann_json::nlohmann_json GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(GtestTests)