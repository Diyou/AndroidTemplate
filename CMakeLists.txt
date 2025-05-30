cmake_minimum_required(VERSION 4.0.0)

set(DOTCMAKE_CONFIGURE_IDE ON)

project(HelloWorld
    VERSION 0.0.1
    DESCRIPTION [=[Hello World C++]=]
    HOMEPAGE_URL https://en.wikipedia.org/wiki/%22Hello,_World!%22_program
    LANGUAGES CXX
)

find_package(SDL3 REQUIRED CONFIG)

include(C++Utils)

EXE(${PROJECT_NAME} Source/main.c++)

target_link_libraries(${PROJECT_NAME}
    .cmake::Utils
    SDL3::SDL3
)
