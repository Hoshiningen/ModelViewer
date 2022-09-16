#pragma once

#include "Camera/Camera.hpp"

#include "Common/ClassMacros.hpp"
#include "Common/Constants.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"

#include "Object/Mesh.hpp"

#include "UI/Components/DirectionalLightProps.hpp"
#include "UI/Components/IComponent.hpp"
#include "UI/Components/LightProps.hpp"
#include "UI/Components/LambertianProps.hpp"
#include "UI/Components/MainMenu.hpp"
#include "UI/Components/ModelLoader.hpp"
#include "UI/Components/ModelProps.hpp"
#include "UI/Components/PhongProps.hpp"
#include "UI/Components/PhongTexturedProps.hpp"
#include "UI/Components/Properties.hpp"
#include "UI/Components/SceneProps.hpp"
#include "UI/Components/SceneTree.hpp"
#include "UI/Components/Viewport.hpp"

#include <array>
#include <string>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sigslot/signal.hpp>

class DirectionalLight;

class MainFrameComponent : public IComponent {
public:
    MainFrameComponent();

    DECLARE_GETTER_MUTABLE(viewport, ViewportComponent)

    sigslot::signal<> exited;
    sigslot::signal<int> themeChanged;

    struct DataModel : public IComponent::DataModel {
        glm::vec3* m_pAmbientColor = nullptr;
        glm::vec4* m_pClearColor = nullptr;
        float* m_pAmbientIntensity = nullptr;

        Mesh* m_pMesh = nullptr;
        std::string m_modelName;

        LambertianMaterial* m_pLambertianMat = nullptr;
        PhongMaterial* m_pPhongMat = nullptr;
        PhongTexturedMaterial* m_pPhongTexturedMat = nullptr;

        std::array<DirectionalLight*, 3> m_lights;
        int* m_pWindowTheme = nullptr;
    };

private:
    virtual void render() override;
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    void OnSceneNodeSelected(SceneTreeComponent::SceneNode node);
    void OnMaterialSelected(int materialIndex);
    void OnModelOpened(const std::forward_list<VertexBuffered>& model, const std::filesystem::path& modelPath);
    void OnModelClosed();
    void OnLightStatusChanged(std::uint8_t lightIndex, bool enabled);

    DataModel m_model;

    // Properties components

    PropertiesComponent m_properties;

    std::array<DirectionalLightProps, 3> m_lightProps;
    
    LambertianProps m_lambertianProps;
    PhongProps m_phongProps;
    PhongTexturedProps m_phongTexturedProps;

    SceneProps m_sceneProps;
    ModelProps m_modelProps;

    LightProps m_lightingProps;

    // Window components

    SceneTreeComponent m_sceneTree;
    ViewportComponent m_viewport;
    MainMenuComponent m_mainMenu;

    // Modal

    ModelLoaderComponent m_modelLoader;
};
