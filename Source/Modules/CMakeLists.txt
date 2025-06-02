find_package(SDL3 REQUIRED CONFIG)
include(C++Utils)

STATIC(${PROJECT_NAME}.modules
PUBLIC
    FILE_SET module_files
    TYPE CXX_MODULES
    FILES
        App.c++
        Logger.c++
        Events.c++
        Windows/Default.c++
)

target_link_libraries(${PROJECT_NAME}.modules
PUBLIC
    .cmake::Utils
    SDL3::SDL3
)

target_compile_definitions(${PROJECT_NAME}.modules PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_VERSION="${PROJECT_VERSION}"
    PROJECT_ID="${PROJECT_ID}"
)

ALIAS(${PROJECT_NAME}.modules ${PROJECT_NAME}::MODULES)
