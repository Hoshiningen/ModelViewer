set(SOURCES
    Framebuffer.cpp
    Renderer.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Renderer/Renderer/Renderer.hpp
    Framebuffer.hpp
    ShaderCache.hpp
)

add_library(Renderer ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Renderer ALIAS Renderer)

# Setup target interface
target_include_directories(Renderer PRIVATE .)
target_include_directories(Renderer PUBLIC ${PUBLIC_DIR}/Renderer)

target_link_libraries(Renderer PRIVATE
    ${PROJECT_NAME}::Camera
    ${PROJECT_NAME}::Common
    ${PROJECT_NAME}::Light
    ${PROJECT_NAME}::Object
    ${PROJECT_NAME}::Shader
    CONAN_PKG::glad
)
  
target_link_libraries(Renderer PUBLIC
    CONAN_PKG::glm
)
