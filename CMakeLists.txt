cmake_minimum_required(VERSION 3.20)
project(ModelViewer)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup(TARGETS)

set(PUBLIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/public)
set(IMGUI_BINDINGS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bindings)
set(FONTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fonts)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(source/Application)
add_subdirectory(source/Camera)
add_subdirectory(source/Common)
add_subdirectory(source/Controls)
add_subdirectory(source/Geometry)
add_subdirectory(source/IO)
add_subdirectory(source/Light)
add_subdirectory(source/Material)
add_subdirectory(source/Object)
add_subdirectory(source/Renderer)
add_subdirectory(source/Shader)
add_subdirectory(source/Texture)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source/Application PROPERTY VS_STARTUP_PROJECT Application)

set(dir)
get_property(dir GLOBAL PROPERTY VS_STARTUP_PROJECT)

message(STATUS ${dir})
