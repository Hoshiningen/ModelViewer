set(SOURCES
    DirectionalLight.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Light/Light/ILight.hpp
    ${PUBLIC_DIR}/Light/Light/DirectionalLight.hpp
)

add_library(Light ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Light ALIAS Light)

# Setup target interface
target_include_directories(Light PRIVATE .)
target_include_directories(Light PUBLIC ${PUBLIC_DIR}/Light)

target_link_libraries(Light PRIVATE
    ${PROJECT_NAME}::Shader
    CONAN_PKG::glm
)
