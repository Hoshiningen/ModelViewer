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
    UI/LightPropertiesDialog.hpp
    UI/LightPropertiesDialog.cpp
    UI/MaterialPropertiesDialog.hpp
    UI/MaterialPropertiesDialog.cpp
    UI/ModelLoaderDialog.hpp
    UI/ModelLoaderDialog.cpp
    UI/ScenePropertiesDialog.hpp
    UI/ScenePropertiesDialog.cpp
)

source_group(bindings ${IMGUI_BINDINGS_DIR})
source_group(UI UI)

add_executable(Application ${SOURCES} ${INCLUDES} ${IMGUI_BINDINGS} ${UI})

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
    ${PROJECT_NAME}::Renderer
    ${PROJECT_NAME}::Texture
    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::glm
    CONAN_PKG::imgui
    CONAN_PKG::nlohmann_json
    CONAN_PKG::sigslot
)
