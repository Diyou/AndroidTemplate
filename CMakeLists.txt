cmake_minimum_required(VERSION 4.0.0)

set(DOTCMAKE_CONFIGURE_IDE ON)

project(HelloSDL
    VERSION 0.0.1
    DESCRIPTION [=[A multi-platform template for SDL3 apps]=]
    HOMEPAGE_URL https://wiki.libsdl.org/SDL3/FrontPage
    LANGUAGES CXX
)

find_package(SDL3 REQUIRED CONFIG)

include(C++Utils)

EXE(${PROJECT_NAME} Source/main.c++)

target_link_libraries(${PROJECT_NAME}
    .cmake::Utils
    SDL3::SDL3
)
