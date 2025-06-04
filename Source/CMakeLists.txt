find_package(SDL3 REQUIRED CONFIG)
include(C++Utils)

target_sources(${PROJECT_NAME}
PUBLIC
    FILE_SET module_files
    TYPE CXX_MODULES
    FILES
        App.c++
        Logger.c++
        Events.c++
        Windows.ixx
        Windows/Default.c++
PRIVATE
    App.main.c++
)

target_link_libraries(${PROJECT_NAME}
    .cmake::Utils
    SDL3::SDL3
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_VERSION="${PROJECT_VERSION}"
    PROJECT_ID="${PROJECT_ID}"
)
