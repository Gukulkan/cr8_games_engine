cmake_minimum_required(VERSION 3.22)
project(cr8_games_engine VERSION 1.0.1 DESCRIPTION "CR8 Engine core")

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME}
        Logger.cpp
        )

add_library(${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ..
        )

target_compile_features(${PROJECT_NAME}
        PRIVATE
            cxx_std_17
        )