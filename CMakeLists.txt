cmake_minimum_required(VERSION 4.0.0)

set(DOTCMAKE_CONFIGURE_IDE ON)

project(SDLTemplate CXX)

EXE(${PROJECT_NAME})

add_subdirectory(Source)
