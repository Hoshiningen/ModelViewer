#include "UI/Components/MainFrame.hpp"

#include "Common/Math.hpp"

#include "Light/DirectionalLight.hpp"

#include <imgui.h>
#include <imgui_internal.h>

MainFrameComponent::MainFrameComponent() {

    m_sceneTree.nodeSelected.connect(&MainFrameComponent::OnSceneNodeSelected, this);
    m_sceneTree.materialSelected.connect(&MainFrameComponent::OnMaterialSelected, this);
    m_sceneTree.lightStatusChanged.connect(&MainFrameComponent::OnLightStatusChanged, this);

    m_sceneProps.ambientColorChanged.connect([this](const glm::vec3& color) { if (m_model.m_pAmbientColor) *m_model.m_pAmbientColor = color; });
    m_sceneProps.clearColorChanged.connect([this](const glm::vec4& color) { if (m_model.m_pClearColor) *m_model.m_pClearColor = color; });
    m_sceneProps.ambientIntensityChanged.connect([this](float intensity) { if (m_model.m_pAmbientIntensity) *m_model.m_pAmbientIntensity = intensity; });

    m_modelProps.pitchChanged.connect([this](float pitch) { m_model.m_pMesh->pitch(pitch); });
    m_modelProps.yawChanged.connect([this](float yaw) { m_model.m_pMesh->yaw(yaw); });
    m_modelProps.rollChanged.connect([this](float roll) { m_model.m_pMesh->roll(roll); });
    m_modelProps.scaleChanged.connect([this](float scale) { m_model.m_pMesh->scale(scale); });
    m_modelProps.positionOffsetsChanged.connect([this](const glm::vec3& offsets) { m_model.m_pMesh->translate(offsets); });

    m_modelLoader.modelLoaded.connect(&MainFrameComponent::OnModelLoaded, this);

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

        DirectionalLight* pLight = m_model.m_lights.at(lightIndex);
        DirectionalLightProps& props = m_lightProps.at(lightIndex);

        props.colorChanged.connect([pLight](const glm::vec3& color) { pLight->color(color); });
        props.pitchChanged.connect([pLight](float pitch) { pLight->pitch(pitch); });
        props.yawChanged.connect([pLight](float yaw) { pLight->yaw(yaw); });
        props.intensityChanged.connect([pLight](float intensity) { pLight->intensity(intensity); });
    }
}

void MainFrameComponent::render() {

    const ImGuiViewport* pViewport = ImGui::GetMainViewport();
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

    static_cast<IComponent&>(m_titleBar).render();
    static_cast<IComponent&>(m_properties).render();
    static_cast<IComponent&>(m_sceneTree).render();
    static_cast<IComponent&>(m_viewport).render();
    static_cast<IComponent&>(m_modelLoader).render();

    ImGui::End();
}

void MainFrameComponent::syncFrom(const std::any& dataModel) {

    if (dataModel.type() != typeid(Model))
        return;

    m_model = std::any_cast<Model>(dataModel);

    m_modelProps.syncFrom(ModelProps::Model{
        .m_scale = m_model.m_pMesh->scale(),
        .m_pitch = m_model.m_pMesh->pitch(),
        .m_yaw = m_model.m_pMesh->yaw(),
        .m_roll = m_model.m_pMesh->roll(),
        .m_offsets = m_model.m_pMesh->translate(),
        .m_origin = m_model.m_pMesh->position(),
        .m_metadata = {
            .faceCount = m_model.m_pMesh->faceCount(),
            .vertexCount = m_model.m_pMesh->vertexCount(),
            .attributes = {
                m_model.m_pMesh->hasColors(),
                m_model.m_pMesh->hasIndices(),
                m_model.m_pMesh->hasNormals(),
                m_model.m_pMesh->hasPositions(),
                m_model.m_pMesh->hasTexels()
            }
        }
    });

    for (std::uint8_t lightIndex = 0; lightIndex != m_model.m_lights.size(); ++lightIndex) {

        DirectionalLight* pLight = m_model.m_lights.at(lightIndex);
        if (!pLight)
            continue;

        m_lightProps.at(lightIndex).syncFrom(DirectionalLightProps::Model{
            .m_enabled = pLight->enabled(),
            .m_color = pLight->color(),
            .m_direction = pLight->direction(),
            .m_pitch = pLight->pitch(),
            .m_yaw = pLight->yaw(),
            .m_intensity = pLight->intensity()
        });
    }

    m_sceneProps.syncFrom(SceneProps::Model{
        .m_ambientColor = *m_model.m_pAmbientColor,
        .m_clearColor = *m_model.m_pClearColor,
        .m_ambientIntensity = *m_model.m_pAmbientIntensity
    });

    m_lambertianProps.syncFrom(LambertianProps::Model{
        .m_diffuseColor = m_model.m_pLambertianMat->diffuseColor(),
        .m_diffuseIntensity = m_model.m_pLambertianMat->diffuseIntensity()
    });

    m_phongProps.syncFrom(PhongProps::Model{
        .m_ambientColor = m_model.m_pPhongMat->ambientColor(),
        .m_diffuseColor = m_model.m_pPhongMat->diffuseColor(),
        .m_specularColor = m_model.m_pPhongMat->specularColor(),
        .m_ambientIntensity = m_model.m_pPhongMat->ambientIntensity(),
        .m_diffuseIntensity = m_model.m_pPhongMat->diffuseIntensity(),
        .m_specularIntensity = m_model.m_pPhongMat->specularIntensity(),
        .m_shininess = m_model.m_pPhongMat->shininess()
    });

    m_phongTexturedProps.syncFrom(PhongTexturedProps::Model{
        .m_diffuseIntensity = m_model.m_pPhongTexturedMat->diffuseIntensity(),
        .m_emissiveIntensity = m_model.m_pPhongTexturedMat->emissiveIntensity(),
        .m_specularIntensity = m_model.m_pPhongTexturedMat->specularIntensity(),
        .m_shininess = m_model.m_pPhongTexturedMat->shininess()
    });
}

DEFINE_GETTER_MUTABLE(MainFrameComponent, viewport, ViewportComponent, m_viewport)

void MainFrameComponent::OnSceneNodeSelected(SceneTreeComponent::SceneNode node) {

    using enum SceneTreeComponent::SceneNode;

    IComponent* pItemProps = nullptr;
    switch (node) {
        case Camera: break;
            break;
        case Light1:
            pItemProps = &m_lightProps.at(0);
            break;
        case Light2:
            pItemProps = &m_lightProps.at(1);
            break;
        case Light3:
            pItemProps = &m_lightProps.at(2);
            break;
        case Material:
            if (dynamic_cast<LambertianMaterial*>(m_model.m_pMesh->material()))
                pItemProps = &m_lambertianProps;

            if (dynamic_cast<PhongMaterial*>(m_model.m_pMesh->material()))
                pItemProps = &m_phongProps;

            if (dynamic_cast<PhongTexturedMaterial*>(m_model.m_pMesh->material()))
                pItemProps = &m_phongTexturedProps;

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

void MainFrameComponent::OnMaterialSelected(int materialIndex, SceneTreeComponent::SceneNode selectedNode) {

    using enum SceneTreeComponent::SceneNode;

    switch (materialIndex) {
    case 0:
        m_model.m_pMesh->material(m_model.m_pLambertianMat);
        if (selectedNode == Material)
            m_properties.propertiesComponent(&m_lambertianProps);

        break;
    case 1:
        m_model.m_pMesh->material(m_model.m_pPhongMat);
        if (selectedNode == Material)
            m_properties.propertiesComponent(&m_phongProps);

        break;
    case 2:
        m_model.m_pMesh->material(m_model.m_pPhongTexturedMat);
        if (selectedNode == Material)
            m_properties.propertiesComponent(&m_phongTexturedProps);

        break;
    default: break;
    }
}

void MainFrameComponent::OnModelLoaded(const std::forward_list<VertexBuffered>& model) {

    m_model.m_pMesh->model(model);
    m_model.m_pMesh->position(-ComputeCenter(m_model.m_pMesh->model()));
    m_model.m_pMesh->scale(ComputeScale(m_model.m_pMesh->model(), kMaxModelSize));
}

void MainFrameComponent::OnLightStatusChanged(std::uint8_t lightIndex, bool enabled) {

    DirectionalLight* pLight = m_model.m_lights.at(lightIndex);
    pLight->enabled(enabled);

    m_lightProps.at(lightIndex).syncFrom(DirectionalLightProps::Model{
        .m_enabled = pLight->enabled(),
        .m_color = pLight->color(),
        .m_direction = pLight->direction(),
        .m_pitch = pLight->pitch(),
        .m_yaw = pLight->yaw(),
        .m_intensity = pLight->intensity(),
    });
}
