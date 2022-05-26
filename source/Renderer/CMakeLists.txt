set(SOURCES
    Renderer.cpp
    GeometryArtist.cpp
    IndexedArtist.cpp
    NonIndexedArtist.cpp
    ShearArtist.cpp
)
    
    set(INCLUDES
    ${PUBLIC_DIR}/Renderer/Renderer/Renderer.hpp
    GeometryArtist.hpp
    IndexedArtist.hpp
    NonIndexedArtist.hpp
    ShearArtist.hpp
)

add_library(Renderer ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Renderer ALIAS Renderer)

# Setup target interface
target_include_directories(Renderer PRIVATE .)
target_include_directories(Renderer PUBLIC ${PUBLIC_DIR}/Renderer)

target_link_libraries(Renderer PRIVATE
    CONAN_PKG::glad
    CONAN_PKG::glm
    ${PROJECT_NAME}::Geometry
    ${PROJECT_NAME}::Camera
    ${PROJECT_NAME}::Shader
)
