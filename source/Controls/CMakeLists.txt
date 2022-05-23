set(SOURCES
    OrbitalControls.cpp
    WindowCallbacks.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Controls/Controls/OrbitalControls.hpp
    ${PUBLIC_DIR}/Controls/Controls/windowCallbacks.hpp
)

add_library(Controls ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Controls ALIAS Controls)

# Setup target interface
target_include_directories(Controls PRIVATE .)
target_include_directories(Controls PUBLIC ${PUBLIC_DIR}/Controls)

target_link_libraries(Controls PRIVATE
    CONAN_PKG::glad
    CONAN_PKG::glm
    CONAN_PKG::glfw
    CONAN_PKG::sigslot
    ${PROJECT_NAME}::Camera
)
