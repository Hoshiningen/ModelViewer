#include "UI/Components/MainFrame.hpp"

#include "Common/Math.hpp"

#include "Light/DirectionalLight.hpp"

#include <imgui.h>
#include <imgui_internal.h>

MainFrameComponent::MainFrameComponent() {

    m_sceneTree.nodeSelected.connect(&MainFrameComponent::OnSceneNodeSelected, this);

    m_modelProps.pitchChanged.connect([this](float pitch) { m_pMesh->pitch(pitch); });
    m_modelProps.yawChanged.connect([this](float yaw) { m_pMesh->yaw(yaw); });
    m_modelProps.rollChanged.connect([this](float roll) { m_pMesh->roll(roll); });
    m_modelProps.scaleChanged.connect([this](float scale) { m_pMesh->scale(scale); });
    m_modelProps.positionOffsetsChanged.connect([this](const glm::vec3& offsets) { m_pMesh->translate(offsets); });

    m_modelLoader.modelLoaded.connect([this](const std::forward_list<VertexBuffered>& model) {
        m_pMesh->model(model);
        m_pMesh->position(-ComputeCenter(m_pMesh->model()));
        m_pMesh->scale(ComputeScale(m_pMesh->model(), kMaxModelSize));
    });

    m_phongProps.ambientColorChanged.connect([this](const glm::vec4& color) { m_pPhongMat->ambientColor(color); });
    m_phongProps.diffuseColorChanged.connect([this](const glm::vec4& color) { m_pPhongMat->diffuseColor(color); });
    m_phongProps.specularColorChanged.connect([this](const glm::vec4& color) { m_pPhongMat->specularColor(color); });
    m_phongProps.ambientIntensityChanged.connect([this](float intensity) { m_pPhongMat->ambientIntensity(intensity); });
    m_phongProps.diffuseIntensityChanged.connect([this](float intensity) { m_pPhongMat->diffuseIntensity(intensity); });
    m_phongProps.specularIntensityChanged.connect([this](float intensity) { m_pPhongMat->specularIntensity(intensity); });
    m_phongProps.shininessChanged.connect([this](float shininess) { m_pPhongMat->shininess(shininess); });
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

DEFINE_SETTER_COPY(MainFrameComponent, mesh, m_pMesh)

DEFINE_SETTER_COPY(MainFrameComponent, lambertianMaterial, m_pLambertianMat)
DEFINE_SETTER_COPY(MainFrameComponent, phongMaterial, m_pPhongMat)
DEFINE_SETTER_COPY(MainFrameComponent, phongTexturedMaterial, m_pPhongTexturedMat)

void MainFrameComponent::directionalLights(const std::vector<DirectionalLight*>& lights) {

    m_lights = lights;

    m_lights.at(0)->enabled(true);
    m_lights.at(1)->enabled(true);

    m_light1Props.colorChanged.connect([this](const glm::vec3& color) { m_lights.at(0)->color(color); });
    m_light1Props.directionChanged.connect([this](const glm::vec3& direction) { m_lights.at(0)->direction(direction); });
    m_light1Props.intensityChanged.connect([this](float intensity) { m_lights.at(0)->intensity(intensity); });

    m_light2Props.colorChanged.connect([this](const glm::vec3& color) { m_lights.at(1)->color(color); });
    m_light2Props.directionChanged.connect([this](const glm::vec3& direction) { m_lights.at(1)->direction(direction); });
    m_light2Props.intensityChanged.connect([this](float intensity) { m_lights.at(1)->intensity(intensity); });

    m_light3Props.colorChanged.connect([this](const glm::vec3& color) { m_lights.at(2)->color(color); });
    m_light3Props.directionChanged.connect([this](const glm::vec3& direction) { m_lights.at(2)->direction(direction); });
    m_light3Props.intensityChanged.connect([this](float intensity) { m_lights.at(2)->intensity(intensity); });
}

DEFINE_GETTER_MUTABLE(MainFrameComponent, viewport, ViewportComponent, m_viewport)

void MainFrameComponent::OnSceneNodeSelected(SceneTreeComponent::SceneNode node) {

    using enum SceneTreeComponent::SceneNode;

    IComponent* pItemProps = nullptr;
    std::string itemName;

    switch (node) {
        case Camera: break;
            itemName = "Camera";
            break;
        case Light1:
            pItemProps = &m_light1Props;
            itemName = "Directional Light 1";
            break;
        case Light2:
            pItemProps = &m_light2Props;
            itemName = "Directional Light 2";
            break;
        case Light3:
            pItemProps = &m_light3Props;
            itemName = "Directional Light 3";
            break;
        case Material:
            pItemProps = &m_phongProps;
            itemName = "Material";
            break;
        case Model:
            pItemProps = &m_modelProps;
            itemName = "Model";
            break;
        case Scene:
            pItemProps = &m_sceneProps;
            itemName = "Scene";
            break;
    }

    m_properties.itemProperties(pItemProps);
    m_properties.itemName(itemName);
}
