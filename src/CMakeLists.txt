cmake_minimum_required(VERSION 3.27)

set(Src
        "converterjson.cpp"
        "searchServer.cpp"
        "invertedIndex.cpp"
        "main.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(Search_Engine ${Src})
target_include_directories(Search_Engine PUBLIC ${MY_INCLUDE_DIR})
target_link_libraries(Search_Engine PRIVATE nlohmann_json::nlohmann_json)