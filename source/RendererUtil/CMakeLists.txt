set(SOURCES Shader.cpp Camera.cpp PerspectiveCamera.cpp OrthographicCamera.cpp)
set(INCLUDES Shader.hpp Camera.hpp PerspectiveCamera.hpp OrthographicCamera.hpp)

add_library(RendererUtil ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::RendererUtil ALIAS RendererUtil)

target_link_libraries(RendererUtil PUBLIC
    CONAN_PKG::glad
    CONAN_PKG::glm
    CONAN_PKG::sigslot
)
