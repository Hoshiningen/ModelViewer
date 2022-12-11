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
    UI/Icons.hpp
    UI/Utility.cpp
    UI/Utility.hpp
)

set(COMPONENTS
    UI/Components/FileExplorer/DirectoryTree.hpp
    UI/Components/FileExplorer/DirectoryTree.cpp
    UI/Components/FileExplorer/DirectoryView.hpp
    UI/Components/FileExplorer/DirectoryView.cpp
    UI/Components/FileExplorer/FileSelector.hpp
    UI/Components/FileExplorer/FileSelector.cpp
    UI/Components/FileExplorer/FileExplorer.hpp
    UI/Components/FileExplorer/FileExplorer.cpp
    UI/Components/Properties/DirectionalLightProps.hpp
    UI/Components/Properties/DirectionalLightProps.cpp
    UI/Components/Properties/LambertianProps.hpp
    UI/Components/Properties/LambertianProps.cpp
    UI/Components/Properties/LightProps.hpp
    UI/Components/Properties/LightProps.cpp
    UI/Components/Properties/ModelProps.hpp
    UI/Components/Properties/ModelProps.cpp
    UI/Components/Properties/PhongProps.hpp
    UI/Components/Properties/PhongProps.cpp
    UI/Components/Properties/PhongTexturedProps.hpp
    UI/Components/Properties/PhongTexturedProps.cpp
    UI/Components/Properties/Properties.hpp
    UI/Components/Properties/Properties.cpp
    UI/Components/Properties/SceneProps.hpp
    UI/Components/Properties/SceneProps.cpp
    UI/Components/IComponent.hpp
    UI/Components/MainFrame.hpp
    UI/Components/MainFrame.cpp
    UI/Components/MainMenu.hpp
    UI/Components/MainMenu.cpp
    UI/Components/SceneTree.hpp
    UI/Components/SceneTree.cpp
    UI/Components/TextureImporter.hpp
    UI/Components/TextureImporter.cpp
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

target_compile_definitions(Application PRIVATE
  $<$<CONFIG:Debug>:MV_DEBUG>
  $<$<CONFIG:RelWithDebInfo>:MV_DEBUG>
)

# Copy shaders for vs execution
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
${FONTS_DIR} ${CMAKE_BINARY_DIR}/source/Application/fonts)