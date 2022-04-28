cmake_minimum_required(VERSION 3.20)
project(ModelViewer)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup(TARGETS)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_subdirectory(source)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${PROJECT_NAME})
