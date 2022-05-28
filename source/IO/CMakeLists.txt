set(SOURCES
    GeometryLoader.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/IO/IO/GeometryLoader.hpp
)

add_library(IO ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::IO ALIAS IO)

# Setup target interface
target_include_directories(IO PRIVATE .)
target_include_directories(IO PUBLIC ${PUBLIC_DIR}/IO)

target_link_libraries(IO PRIVATE
    ${PROJECT_NAME}::Geometry
    CONAN_PKG::assimp
    CONAN_PKG::glm
)
