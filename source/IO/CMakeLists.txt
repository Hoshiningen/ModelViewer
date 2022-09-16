set(SOURCES
    GeometryLoader.cpp
    TextureLoader.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/IO/IO/GeometryLoader.hpp
    ${PUBLIC_DIR}/IO/IO/TextureLoader.hpp
)

add_library(IO ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::IO ALIAS IO)

# Setup target interface
target_include_directories(IO PRIVATE .)
target_include_directories(IO PUBLIC ${PUBLIC_DIR}/IO)

target_link_libraries(IO PRIVATE
    ${PROJECT_NAME}::Common
    ${PROJECT_NAME}::Renderer
    CONAN_PKG::assimp
    CONAN_PKG::glm
    CONAN_PKG::stb
)

target_link_libraries(IO PUBLIC
    ${PROJECT_NAME}::Geometry
    ${PROJECT_NAME}::Texture
)