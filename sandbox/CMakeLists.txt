cmake_minimum_required(VERSION 3.22)
project(cr8_games_sandbox)

set(CMAKE_CXX_STANDARD 23)

add_executable(cr8_games_sandbox main.cpp SandboxApp.cpp SandboxApp.h)

FILE(COPY shader/simple_shader.frag.spv DESTINATION "${CMAKE_BINARY_DIR}/sandbox/shader")
FILE(COPY shader/simple_shader.vert.spv DESTINATION "${CMAKE_BINARY_DIR}/sandbox/shader")
FILE(COPY models/smooth_vase.obj DESTINATION "${CMAKE_BINARY_DIR}/sandbox/models/")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            engine
        )