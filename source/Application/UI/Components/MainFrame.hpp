#pragma once

#include "Common/ClassMacros.hpp"
#include "Common/Constants.hpp"

#include "Material/LambertianMaterial.hpp"
#include "Material/PhongMaterial.hpp"
#include "Material/PhongTexturedMaterial.hpp"

#include "Object/Mesh.hpp"

#include "UI/Components/DirectionalLightProps.hpp"
#include "UI/Components/IComponent.hpp"
#include "UI/Components/LambertianProps.hpp"
#include "UI/Components/ModelLoader.hpp"
#include "UI/Components/ModelProps.hpp"
#include "UI/Components/PhongProps.hpp"
#include "UI/Components/PhongTexturedProps.hpp"
#include "UI/Components/Properties.hpp"
#include "UI/Components/SceneProps.hpp"
#include "UI/Components/SceneTree.hpp"
#include "UI/Components/TitleBar.hpp"
#include "UI/Components/Viewport.hpp"

#include <array>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sigslot/signal.hpp>

class DirectionalLight;

class MainFrameComponent : public IComponent {
public:
    struct Model {
        glm::vec3* m_pAmbientColor = nullptr;
        glm::vec4* m_pClearColor = nullptr;
        float* m_pAmbientIntensity = nullptr;

        Mesh* m_pMesh = nullptr;

        LambertianMaterial* m_pLambertianMat = nullptr;
        PhongMaterial* m_pPhongMat = nullptr;
        PhongTexturedMaterial* m_pPhongTexturedMat = nullptr;

        std::array<DirectionalLight*, 3> m_lights;
    };

    MainFrameComponent();

    virtual void render() override;
    virtual void syncFrom(const std::any& dataModel) override;

    DECLARE_GETTER_MUTABLE(viewport, ViewportComponent)
    //DECLARE_GETTER_MUTABLE(titleBar, TitleBarComponent)

private:
    void OnSceneNodeSelected(SceneTreeComponent::SceneNode node);
    void OnMaterialSelected(int materialIndex, SceneTreeComponent::SceneNode selectedNode);
    void OnModelLoaded(const std::forward_list<VertexBuffered>& model);
    void OnLightStatusChanged(std::uint8_t lightIndex, bool enabled);

    Model m_model;

    // Properties components

    PropertiesComponent m_properties;

    std::array<DirectionalLightProps, 3> m_lightProps;
    
    LambertianProps m_lambertianProps;
    PhongProps m_phongProps;
    PhongTexturedProps m_phongTexturedProps;

    SceneProps m_sceneProps;
    ModelProps m_modelProps;

    // Window components

    SceneTreeComponent m_sceneTree;
    ViewportComponent m_viewport;
    TitleBarComponent m_titleBar;

    // Modal

    ModelLoaderComponent m_modelLoader;
};
