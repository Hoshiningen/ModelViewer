set(SOURCES
    Application.cpp
    main.cpp
)

set(INCLUDES
    Application.hpp
)

add_executable(Application ${SOURCES} ${INCLUDES})

# Setup target interface
target_include_directories(Application PRIVATE .)

target_link_libraries(Application PRIVATE
    ${PROJECT_NAME}::Camera
    ${PROJECT_NAME}::Controls
    ${PROJECT_NAME}::Geometry
    ${PROJECT_NAME}::IO
    ${PROJECT_NAME}::Renderer
    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::glm
    CONAN_PKG::sigslot
)
