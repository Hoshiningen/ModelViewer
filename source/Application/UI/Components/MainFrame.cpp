#include "UI/Components/MainFrame.hpp"

#include "Common/Math.hpp"

#include "Light/DirectionalLight.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>

MainFrameComponent::MainFrameComponent() {

    m_sceneTree.nodeSelected.connect(&MainFrameComponent::OnSceneNodeSelected, this);
    m_sceneTree.materialSelected.connect([this](int materialIndex) {
        OnMaterialSelected(materialIndex);
        static_cast<IComponent&>(m_modelProps).syncFrom(dataModel());
    });
    m_sceneTree.lightStatusChanged.connect(&MainFrameComponent::OnLightStatusChanged, this);
    m_sceneTree.modelClosed.connect(&MainFrameComponent::OnModelClosed, this);

    m_sceneProps.ambientColorChanged.connect([this](const glm::vec3& color) { if (m_model.m_pAmbientColor) *m_model.m_pAmbientColor = color; });
    m_sceneProps.clearColorChanged.connect([this](const glm::vec4& color) { if (m_model.m_pClearColor) *m_model.m_pClearColor = color; });
    m_sceneProps.ambientIntensityChanged.connect([this](float intensity) { if (m_model.m_pAmbientIntensity) *m_model.m_pAmbientIntensity = intensity; });

    m_modelProps.pitchChanged.connect([this](float pitch) { m_model.m_pMesh->pitch(pitch); });
    m_modelProps.yawChanged.connect([this](float yaw) { m_model.m_pMesh->yaw(yaw); });
    m_modelProps.rollChanged.connect([this](float roll) { m_model.m_pMesh->roll(roll); });
    m_modelProps.scaleChanged.connect([this](float scale) { m_model.m_pMesh->scale(scale); });
    m_modelProps.positionOffsetsChanged.connect([this](const glm::vec3& offsets) { m_model.m_pMesh->translate(offsets); });
    m_modelProps.materialSelected.connect([this](int materialIndex) {
        OnMaterialSelected(materialIndex);
        static_cast<IComponent&>(m_sceneTree).syncFrom(dataModel());
    });

    m_modelLoader.modelOpened.connect(&MainFrameComponent::OnModelOpened, this);

    m_lambertianProps.diffuseColorChanged.connect([this](const glm::vec4& color) { m_model.m_pLambertianMat->diffuseColor(color); });
    m_lambertianProps.diffuseIntensityChanged.connect([this](float intensity) { m_model.m_pLambertianMat->diffuseIntensity(intensity); });

    m_phongProps.ambientColorChanged.connect([this](const glm::vec4& color) { m_model.m_pPhongMat->ambientColor(color); });
    m_phongProps.diffuseColorChanged.connect([this](const glm::vec4& color) { m_model.m_pPhongMat->diffuseColor(color); });
    m_phongProps.specularColorChanged.connect([this](const glm::vec4& color) { m_model.m_pPhongMat->specularColor(color); });
    m_phongProps.ambientIntensityChanged.connect([this](float intensity) { m_model.m_pPhongMat->ambientIntensity(intensity); });
    m_phongProps.diffuseIntensityChanged.connect([this](float intensity) { m_model.m_pPhongMat->diffuseIntensity(intensity); });
    m_phongProps.specularIntensityChanged.connect([this](float intensity) { m_model.m_pPhongMat->specularIntensity(intensity); });
    m_phongProps.shininessChanged.connect([this](float shininess) { m_model.m_pPhongMat->shininess(shininess); });

    m_phongTexturedProps.diffuseIntensityChanged.connect([this](float intensity) { m_model.m_pPhongTexturedMat->diffuseIntensity(intensity); });
    m_phongTexturedProps.diffuseMapLoaded.connect([this](const Texture& map) { m_model.m_pPhongTexturedMat->diffuseMap(map); });
    m_phongTexturedProps.emissiveIntensityChanged.connect([this](float intensity) { m_model.m_pPhongTexturedMat->emissiveIntensity(intensity); });
    m_phongTexturedProps.emissiveMapLoaded.connect([this](const Texture& map) { m_model.m_pPhongTexturedMat->emissiveMap(map); });
    m_phongTexturedProps.specularIntensityChanged.connect([this](float intensity) { m_model.m_pPhongTexturedMat->specularIntensity(intensity); });
    m_phongTexturedProps.specularMapLoaded.connect([this](const Texture& map) { m_model.m_pPhongTexturedMat->specularMap(map); });
    m_phongTexturedProps.shininessChanged.connect([this](float shininess) { m_model.m_pPhongTexturedMat->shininess(shininess); });

    for (std::uint8_t lightIndex = 0; lightIndex < m_model.m_lights.size(); ++lightIndex) {
        DirectionalLightProps& props = m_lightProps.at(lightIndex);

        props.colorChanged.connect([this, lightIndex](const glm::vec3& color) { m_model.m_lights.at(lightIndex)->color(color); });
        props.pitchChanged.connect([this, lightIndex](float pitch) { m_model.m_lights.at(lightIndex)->pitch(pitch); });
        props.yawChanged.connect([this, lightIndex](float yaw) { m_model.m_lights.at(lightIndex)->yaw(yaw); });
        props.intensityChanged.connect([this, lightIndex](float intensity) { m_model.m_lights.at(lightIndex)->intensity(intensity); });
        props.enabledChanged.connect([this, lightIndex](bool enabled) {
            m_model.m_lights.at(lightIndex)->enabled(enabled);
            static_cast<IComponent&>(m_sceneTree).syncFrom(dataModel());
        });
    }

    m_mainMenu.exited.connect([this] { exited(); });
    m_mainMenu.modelClosed.connect(&MainFrameComponent::OnModelClosed, this);
    m_mainMenu.lightPropertiesOpened.connect([this] { OnSceneNodeSelected(SceneTreeComponent::SceneNode::Lighting); });
    m_mainMenu.modelPropertiesOpened.connect([this] { OnSceneNodeSelected(SceneTreeComponent::SceneNode::Model); });
    m_mainMenu.scenePropertiesOpened.connect([this] { OnSceneNodeSelected(SceneTreeComponent::SceneNode::Scene); });
    m_mainMenu.themeChanged.connect([this](int theme) { themeChanged(theme); });
}

void MainFrameComponent::render() {

    const ImGuiViewport* pViewport = ImGui::GetMainViewport();

    // Offset the GUI by the height of the menu bar.
    ImGui::SetNextWindowPos({ pViewport->Pos.x, pViewport->Pos.y + ImGui::GetFrameHeight() });
    ImGui::SetNextWindowSize(pViewport->Size);
    ImGui::SetNextWindowViewport(pViewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });

    const ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoNavFocus;

    const ImGuiDockNodeFlags dockspaceFlags =
        ImGuiDockNodeFlags_PassthruCentralNode |
        ImGuiDockNodeFlags_NoTabBar |
        ImGuiDockNodeFlags_NoSplit;

    ImGui::Begin("Main Frame", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    const ImGuiID dockspaceId = ImGui::GetID("MainFrameDockspace");
    ImGui::DockSpace(dockspaceId, { 0, 0 }, dockspaceFlags);

    static bool runOnce = true;
    if (runOnce) {
        runOnce = false;

        ImGui::DockBuilderRemoveNode(dockspaceId);
        ImGui::DockBuilderAddNode(dockspaceId, dockspaceFlags);
        ImGui::DockBuilderSetNodeSize(dockspaceId, pViewport->Size);

        ImGuiID mainRight;
        const ImGuiID mainLeft = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, .20f, nullptr, &mainRight);

        ImGuiID mainLeftBottom;
        const ImGuiID mainLeftTop = ImGui::DockBuilderSplitNode(mainLeft, ImGuiDir_Up, .22f, nullptr, &mainLeftBottom);

        ImGui::DockBuilderDockWindow(static_cast<IComponent&>(m_properties).windowId(), mainLeftBottom);
        ImGui::DockBuilderDockWindow(static_cast<IComponent&>(m_sceneTree).windowId(), mainLeftTop);
        ImGui::DockBuilderDockWindow(static_cast<IComponent&>(m_viewport).windowId(), mainRight);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    static_cast<IComponent&>(m_mainMenu).render();
    static_cast<IComponent&>(m_properties).render();
    static_cast<IComponent&>(m_sceneTree).render();
    static_cast<IComponent&>(m_viewport).render();
    static_cast<IComponent&>(m_modelLoader).render();

    ImGui::End();
}

void MainFrameComponent::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const DataModel*>(pFrom);
    if (!pModel)
        return;

    m_model = *pModel;

    for (std::uint8_t lightIndex = 0; lightIndex != m_model.m_lights.size(); ++lightIndex) {

        DirectionalLight* pLight = m_model.m_lights.at(lightIndex);
        if (!pLight)
            continue;

        DirectionalLightProps::DataModel model;
        model.m_enabled = pLight->enabled();
        model.m_color = pLight->color();
        model.m_direction = pLight->direction();
        model.m_pitch = pLight->pitch();
        model.m_yaw = pLight->yaw();
        model.m_intensity = pLight->intensity();

        static_cast<IComponent&>(m_lightProps.at(lightIndex)).syncFrom(&model);
    }

    static_cast<IComponent&>(m_lightingProps).compose({
        &m_lightProps.at(0),
        &m_lightProps.at(1),
        &m_lightProps.at(2)
    });
    static_cast<IComponent&>(m_lightingProps).syncFrom(dataModel());

    static_cast<IComponent&>(m_modelProps).compose({
        &m_lambertianProps,
        &m_phongProps,
        &m_phongTexturedProps
    });
    static_cast<IComponent&>(m_modelProps).syncFrom(dataModel());

    static_cast<IComponent&>(m_sceneProps).syncFrom(dataModel());
    static_cast<IComponent&>(m_lambertianProps).syncFrom(dataModel());
    static_cast<IComponent&>(m_phongProps).syncFrom(dataModel());
    static_cast<IComponent&>(m_phongTexturedProps).syncFrom(dataModel());

    static_cast<IComponent&>(m_sceneTree).syncFrom(dataModel());
    static_cast<IComponent&>(m_mainMenu).syncFrom(dataModel());
}

const IComponent::DataModel* MainFrameComponent::dataModel() const {
    return &m_model;
}

DEFINE_GETTER_MUTABLE(MainFrameComponent, viewport, ViewportComponent, m_viewport)

void MainFrameComponent::OnSceneNodeSelected(SceneTreeComponent::SceneNode node) {

    using enum SceneTreeComponent::SceneNode;

    IComponent* pItemProps = nullptr;
    switch (node) {
        case Lighting:
            pItemProps = &m_lightingProps;
            break;
        case Light1:
            pItemProps = &m_lightingProps;
            break;
        case Light2:
            pItemProps = &m_lightingProps;
            break;
        case Light3:
            pItemProps = &m_lightingProps;
            break;
        case Material:
            pItemProps = &m_modelProps;
            break;
        case Model:
            pItemProps = &m_modelProps;
            break;
        case Scene:
            pItemProps = &m_sceneProps;
            break;
    }

    m_properties.propertiesComponent(pItemProps);
}

void MainFrameComponent::OnMaterialSelected(int materialIndex) {

    IMaterial* pSelected = nullptr;
    switch (materialIndex) {
    case 0:
        pSelected = m_model.m_pLambertianMat;
        break;
    case 1:
        pSelected = m_model.m_pPhongMat;
        break;
    case 2:
        pSelected = m_model.m_pPhongTexturedMat;
        break;
    default: break;
    }

    m_model.m_pMesh->material(pSelected);
}

void MainFrameComponent::OnModelOpened(const std::forward_list<VertexBuffered>& model, const std::filesystem::path& modelPath) {

    m_model.m_pMesh->destroy();
    m_model.m_pMesh->model(model);
    m_model.m_pMesh->position(-ComputeCenter(m_model.m_pMesh->model()));
    m_model.m_pMesh->scale(ComputeScale(m_model.m_pMesh->model(), kMaxModelSize));
    m_model.m_modelName = modelPath.filename().string();

    static_cast<IComponent&>(m_modelProps).syncFrom(dataModel());
    static_cast<IComponent&>(m_sceneTree).syncFrom(dataModel());
    static_cast<IComponent&>(m_mainMenu).syncFrom(dataModel());
}

void MainFrameComponent::OnLightStatusChanged(std::uint8_t lightIndex, bool enabled) {

    DirectionalLight* pLight = m_model.m_lights.at(lightIndex);
    pLight->enabled(enabled);

    DirectionalLightProps::DataModel model;
    model.m_enabled = pLight->enabled();
    model.m_color = pLight->color();
    model.m_direction = pLight->direction();
    model.m_pitch = pLight->pitch();
    model.m_yaw = pLight->yaw();
    model.m_intensity = pLight->intensity();

    static_cast<IComponent&>(m_lightProps.at(lightIndex)).syncFrom(&model);
}

void MainFrameComponent::OnModelClosed() {

    m_properties.propertiesComponent(nullptr);
    m_model.m_pMesh->destroy();

    static_cast<IComponent&>(m_sceneTree).syncFrom(dataModel());
    static_cast<IComponent&>(m_mainMenu).syncFrom(dataModel());
}
