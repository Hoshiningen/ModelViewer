set(SOURCES
    Application.cpp
    main.cpp
)

set(INCLUDES
    Application.hpp 
)

set(IMGUI_BINDINGS
    ${IMGUI_BINDINGS_DIR}/imgui_impl_glfw.h
    ${IMGUI_BINDINGS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BINDINGS_DIR}/imgui_impl_opengl3_loader.h
    ${IMGUI_BINDINGS_DIR}/imgui_impl_opengl3.h
    ${IMGUI_BINDINGS_DIR}/imgui_impl_opengl3.cpp
)

set(UI
    UI/Dialog.hpp
    UI/Dialog.cpp
    UI/Icons.hpp
    UI/Utility.cpp
    UI/Utility.hpp
    UI/LightPropertiesDialog.hpp
    UI/LightPropertiesDialog.cpp
    UI/MaterialPropertiesDialog.hpp
    UI/MaterialPropertiesDialog.cpp
    UI/ModelLoaderDialog.hpp
    UI/ModelLoaderDialog.cpp
    UI/ScenePropertiesDialog.hpp
    UI/ScenePropertiesDialog.cpp
)

set(COMPONENTS
    UI/Components/DirectionalLightProps.hpp
    UI/Components/DirectionalLightProps.cpp
    UI/Components/IComponent.hpp
    UI/Components/LambertianProps.hpp
    UI/Components/LambertianProps.cpp
    UI/Components/LightProps.hpp
    UI/Components/LightProps.cpp
    UI/Components/MainFrame.hpp
    UI/Components/MainFrame.cpp
    UI/Components/MainMenu.hpp
    UI/Components/MainMenu.cpp
    UI/Components/ModelLoader.hpp
    UI/Components/ModelLoader.cpp
    UI/Components/ModelProps.hpp
    UI/Components/ModelProps.cpp
    UI/Components/PhongProps.hpp
    UI/Components/PhongProps.cpp
    UI/Components/PhongTexturedProps.hpp
    UI/Components/PhongTexturedProps.cpp
    UI/Components/Properties.hpp
    UI/Components/Properties.cpp
    UI/Components/SceneProps.hpp
    UI/Components/SceneProps.cpp
    UI/Components/SceneTree.hpp
    UI/Components/SceneTree.cpp
    UI/Components/Viewport.hpp
    UI/Components/Viewport.cpp
)
source_group(bindings ${IMGUI_BINDINGS_DIR})
source_group(UI UI)
source_group(UI/Components UI/Components)

add_executable(Application ${SOURCES} ${INCLUDES} ${IMGUI_BINDINGS} ${UI} ${COMPONENTS})

# Setup target interface
target_include_directories(Application PRIVATE . ${IMGUI_BINDINGS_DIR})

target_link_libraries(Application PRIVATE
    ${PROJECT_NAME}::Camera
    ${PROJECT_NAME}::Common
    ${PROJECT_NAME}::Controls
    ${PROJECT_NAME}::Geometry
    ${PROJECT_NAME}::IO
    ${PROJECT_NAME}::Light
    ${PROJECT_NAME}::Material
    ${PROJECT_NAME}::Object
    ${PROJECT_NAME}::Renderer
    ${PROJECT_NAME}::Texture
    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::glm
    CONAN_PKG::imgui
    CONAN_PKG::nlohmann_json
    CONAN_PKG::sigslot
)

# Copy shaders for vs execution
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
${FONTS_DIR} ${CMAKE_BINARY_DIR}/source/Application/fonts)