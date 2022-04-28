set(SOURCES Shader.cpp)
set(INCLUDES Shader.hpp Camera.hpp)

add_library(RendererUtil ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::RendererUtil ALIAS RendererUtil)

target_link_libraries(RendererUtil PUBLIC
    CONAN_PKG::glad
    CONAN_PKG::glm
)
