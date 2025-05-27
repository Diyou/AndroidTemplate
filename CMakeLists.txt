cmake_minimum_required(VERSION 4.0.0 FATAL_ERROR)

set(DOTCMAKE_CONFIGURE_IDE ON)

project(HelloWorld
    VERSION 0.0.1
    DESCRIPTION [=[Hello World C++]=]
    HOMEPAGE_URL https://en.wikipedia.org/wiki/%22Hello,_World!%22_program
    LANGUAGES CXX
)

#EXE(${PROJECT_NAME} Source/main.c++)

#include(C++Utils)
#target_link_libraries(${PROJECT_NAME} PRIVATE .cmake::Utils)

add_library(${PROJECT_NAME} SHARED
    Source/jni.c++
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME native
)

target_link_libraries(${PROJECT_NAME}
    android
    log
)
