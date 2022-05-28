set(SOURCES
    Shader.cpp
    )
    
set(INCLDUES
    ${PUBLIC_DIR}/Shader/Shader/Shader.hpp
)

set(SHADERS
    glsl/shear.frag
    glsl/shear.vert
    glsl/nonIndexed.frag
    glsl/nonIndexed.vert
)

add_library(Shader ${SOURCES} ${INCLDUES} ${SHADERS})
add_library(${PROJECT_NAME}::Shader ALIAS Shader)

source_group(glsl FILES ${SHADERS})
target_sources(Shader PRIVATE ${SHADERS})

# Setup target interface
target_include_directories(Shader PRIVATE .)
target_include_directories(Shader PUBLIC ${PUBLIC_DIR}/Shader)

target_link_libraries(Shader PRIVATE
    CONAN_PKG::glad
    CONAN_PKG::glm
)

# Copy shaders for vs execution
add_custom_command(TARGET Shader POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl ${CMAKE_BINARY_DIR}/source/Application/glsl)

# Copy shaders for renderdoc execution
add_custom_command(TARGET Shader POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl $<TARGET_FILE_DIR:Application>/glsl)
