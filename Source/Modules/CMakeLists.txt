find_package(SDL3 REQUIRED CONFIG)
include(C++Utils)

STATIC(${PROJECT_NAME}_modules
PUBLIC
    FILE_SET module_files
    TYPE CXX_MODULES
    FILES
        Logger.c++
        Window.c++
)

target_link_libraries(${PROJECT_NAME}_modules
PUBLIC
    .cmake::Utils
    SDL3::SDL3
)

ALIAS(${PROJECT_NAME}_modules ${PROJECT_NAME}::MODULES)
