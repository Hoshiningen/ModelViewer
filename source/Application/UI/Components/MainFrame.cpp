#include "UI/Components/MainFrame.hpp"

#include <imgui.h>
#include <imgui_internal.h>

MainFrameComponent::MainFrameComponent() {
    m_sceneTree.nodeSelected.connect(&MainFrameComponent::OnSceneNodeSelected, this);
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
    ImGui::DockSpace(dockspaceId, {0, 0}, dockspaceFlags);

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

        ImGui::DockBuilderDockWindow(m_properties.windowId(), mainLeftBottom);
        ImGui::DockBuilderDockWindow(m_sceneTree.windowId(), mainLeftTop);
        ImGui::DockBuilderDockWindow(m_viewport.windowId(), mainRight);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    m_titleBar.render();
    m_properties.render();
    m_sceneTree.render();
    m_viewport.render();
    m_modelLoader.render();

    ImGui::End();
}

void MainFrameComponent::framebufferTexture(GLuint textureId) {
    m_viewport.framebufferTexture(textureId);
}

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
