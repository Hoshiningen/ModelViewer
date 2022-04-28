set(SOURCES Renderer.cpp)
set(INCLUDES Renderer.hpp)
set(SHADERS glsl/fragment.glsl glsl/vertex.glsl)

add_library(Renderer ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Renderer ALIAS Renderer)

target_sources(Renderer PRIVATE ${SHADERS})
source_group(glsl FILES ${SHADERS})

target_link_libraries(Renderer PRIVATE ${PROJECT_NAME}::RendererUtil)
target_include_directories(Renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Copy shaders for vs execution
add_custom_command(TARGET Renderer POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl ${CMAKE_BINARY_DIR}/source/glsl)

# Copy shaders for renderdoc execution
add_custom_command(TARGET Renderer POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl $<TARGET_FILE_DIR:${PROJECT_NAME}>/glsl)
