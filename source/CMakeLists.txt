set(SOURCES main.cpp App.cpp WindowCallbacks.cpp OrbitalControls.cpp)
set(INCLUDES App.hpp WindowCallbacks.hpp OrbitalControls.hpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE
    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::glm
    CONAN_PKG::nlohmann_json
    CONAN_PKG::sigslot
    ${PROJECT_NAME}::Renderer
)

# Copy the config file to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/config.json ${CMAKE_BINARY_DIR}/source)

add_subdirectory(Renderer)
set_target_properties(Renderer PROPERTIES FOLDER Libraries)

add_subdirectory(RendererUtil)
set_target_properties(RendererUtil PROPERTIES FOLDER Libraries)
