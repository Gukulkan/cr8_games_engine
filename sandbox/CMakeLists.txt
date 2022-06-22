cmake_minimum_required(VERSION 3.22)
project(cr8_games_sandbox)

set(CMAKE_CXX_STANDARD 23)

add_executable(cr8_games_sandbox main.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            engine
        )