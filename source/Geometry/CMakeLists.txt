set(SOURCES
    Box.cpp
    Line.cpp
    Plane.cpp
    Point.cpp
    VertexBuffer.cpp
    VertexBuffered.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Geometry/Geometry/Box.hpp
    ${PUBLIC_DIR}/Geometry/Geometry/Line.hpp
    ${PUBLIC_DIR}/Geometry/Geometry/Plane.hpp
    ${PUBLIC_DIR}/Geometry/Geometry/Point.hpp
    ${PUBLIC_DIR}/Geometry/Geometry/VertexBuffer.hpp
    ${PUBLIC_DIR}/Geometry/Geometry/VertexBuffered.hpp
)

add_library(Geometry ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Geometry ALIAS Geometry)

# Setup target interface
target_include_directories(Geometry PRIVATE .)
target_include_directories(Geometry PUBLIC ${PUBLIC_DIR}/Geometry)

target_link_libraries(Geometry PRIVATE
    CONAN_PKG::glm
)
