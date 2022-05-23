set(SOURCES
    Camera.cpp
    OrthographicCamera.cpp
    PerspectiveCamera.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Camera/Camera/Camera.hpp
    ${PUBLIC_DIR}/Camera/Camera/OrthographicCamera.hpp
    ${PUBLIC_DIR}/Camera/Camera/PerspectiveCamera.hpp
)

add_library(Camera ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Camera ALIAS Camera)

# Setup target interface
target_include_directories(Camera PRIVATE .)
target_include_directories(Camera PUBLIC ${PUBLIC_DIR}/Camera)

target_link_libraries(Camera PRIVATE
    CONAN_PKG::glm
)