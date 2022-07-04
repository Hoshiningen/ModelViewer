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
    ${PROJECT_NAME}::Camera
    CONAN_PKG::glad
    CONAN_PKG::glm
    CONAN_PKG::glfw
)
    
target_link_libraries(Controls PUBLIC
    CONAN_PKG::sigslot
)
