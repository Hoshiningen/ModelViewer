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

#include <vector>

#include <glad/glad.h>
#include <sigslot/signal.hpp>

class DirectionalLight;

class MainFrameComponent : public IComponent {
public:
    MainFrameComponent();

    virtual void render() override;

    DECLARE_SETTER_COPY(mesh, Mesh*)

    DECLARE_SETTER_COPY(lambertianMaterial, LambertianMaterial*)
    DECLARE_SETTER_COPY(phongMaterial, PhongMaterial*)
    DECLARE_SETTER_COPY(phongTexturedMaterial, PhongTexturedMaterial*)

    DECLARE_SETTER_CONSTREF(directionalLights, std::vector<DirectionalLight*>)
 
    DECLARE_GETTER_MUTABLE(viewport, ViewportComponent)
    //DECLARE_GETTER_MUTABLE(titleBar, TitleBarComponent)
    //DECLARE_GETTER_MUTABLE(properties, ViewportComponent)

private:
    void OnSceneNodeSelected(SceneTreeComponent::SceneNode node);

    Mesh* m_pMesh = nullptr;

    LambertianMaterial* m_pLambertianMat = nullptr;
    PhongMaterial* m_pPhongMat = nullptr;
    PhongTexturedMaterial* m_pPhongTexturedMat = nullptr;

    std::vector<DirectionalLight*> m_lights;

    // Properties components

    PropertiesComponent m_properties;

    DirectionalLightProps m_light1Props;
    DirectionalLightProps m_light2Props;
    DirectionalLightProps m_light3Props;
    
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
