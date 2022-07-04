set(SOURCES
    LambertianMaterial.cpp
    MeshMaterial.cpp
    PhongMaterial.cpp
    SolidMaterial.cpp
    SolidPointLineMaterial.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Material/Material/IMaterial.hpp
    ${PUBLIC_DIR}/Material/Material/LambertianMaterial.hpp
    ${PUBLIC_DIR}/Material/Material/MeshMaterial.hpp
    ${PUBLIC_DIR}/Material/Material/PhongMaterial.hpp
    ${PUBLIC_DIR}/Material/Material/SolidMaterial.hpp
    ${PUBLIC_DIR}/Material/Material/SolidPointLineMaterial.hpp
)

add_library(Material ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Material ALIAS Material)

# Setup target interface
target_include_directories(Material PRIVATE .)
target_include_directories(Material PUBLIC ${PUBLIC_DIR}/Material)

target_link_libraries(Material PRIVATE
    ${PROJECT_NAME}::Shader
    ${PROJECT_NAME}::Texture
)

target_link_libraries(Material PUBLIC
    CONAN_PKG::glm
)
