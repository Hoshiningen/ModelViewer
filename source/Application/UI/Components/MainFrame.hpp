#pragma once

#include "Common/ClassMacros.hpp"

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

#include <glad/glad.h>
#include <sigslot/signal.hpp>

class MainFrameComponent : public IComponent {
public:
    MainFrameComponent();

    virtual void render() override;

    DECLARE_SETTER_COPY(framebufferTexture, GLuint)

private:
    void OnSceneNodeSelected(SceneTreeComponent::SceneNode node);

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
