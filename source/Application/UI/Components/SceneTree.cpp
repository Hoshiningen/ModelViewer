#include "UI/Components/SceneTree.hpp"

#include "Light/DirectionalLight.hpp"

#include "UI/Components/MainFrame.hpp"
#include "UI/Components/FileExplorer/FileExplorer.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <format>
#include <iostream>

#include <imgui.h>

ImGuiTreeNodeFlags NodeFlags(bool selected, bool isLeaf) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;
    
    if (isLeaf)
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    else
        nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;

    return selected ? nodeFlags | ImGuiTreeNodeFlags_Selected : nodeFlags;
}

const char* SceneTreeComponent::windowId() const {
    return "SceneTree";
}

void SceneTreeComponent::render() {

    bool openFileExplorer = false;
    ImGui::Begin(windowId());

    if (ImGui::BeginTabBar("SceneTreeTabBar") && ImGui::BeginTabItem("Scene Tree")) {

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        ImGuiTreeNodeFlags nodeFlags = NodeFlags(selected(SceneNode::Scene), false);
        const bool sceneNodeOpen = ImGui::TreeNodeEx(Utility::Label("Scene", CUBES_ICON).c_str(), nodeFlags);

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem(Utility::Label("Load Model...", FOLDER_OPEN_ICON).c_str()))
                openFileExplorer = true;


            if (ImGui::MenuItem("Remove Model", nullptr, nullptr, m_model.m_modelLoaded))
                modelClosed();


            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked())
            select(SceneNode::Scene);

        if (sceneNodeOpen) {

            if (m_model.m_modelLoaded) {

                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                nodeFlags = NodeFlags(selected(SceneNode::Model), false);
                const bool modelNodeOpen = ImGui::TreeNodeEx(Utility::Label(m_model.m_modelName, CUBE_ICON).c_str(), nodeFlags);

                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::BeginMenu("Active Material")) {

                        if (ImGui::RadioButton("Lambertian", &m_model.m_selectedMaterial, 0))
                            materialSelected(m_model.m_selectedMaterial);

                        if (ImGui::RadioButton("Phong", &m_model.m_selectedMaterial, 1))
                            materialSelected(m_model.m_selectedMaterial);

                        if (ImGui::RadioButton("Phong Textured", &m_model.m_selectedMaterial, 2))
                            materialSelected(m_model.m_selectedMaterial);

                        ImGui::EndMenu();
                    }

                    ImGui::EndPopup();
                }

                if (ImGui::IsItemClicked())
                    select(SceneNode::Model);

                if (modelNodeOpen) {
                    const char* materialName = [this] {
                        switch (m_model.m_selectedMaterial) {
                        case 0: return "Lambertian Material";
                        case 1: return "Phong Material";
                        case 2: return "Phong Textured Material";
                        default: return "Material";
                        }
                    }();

                    nodeFlags = NodeFlags(selected(SceneNode::Material), true);
                    ImGui::TreeNodeEx(Utility::Label(materialName, PALETTE_ICON).c_str(), nodeFlags | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    if (ImGui::IsItemClicked())
                        select(SceneNode::Material);

                    ImGui::TreePop();
                }
            }

            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            nodeFlags = NodeFlags(selected(SceneNode::Lighting), false);
            const bool lightingNodeOpen = ImGui::TreeNodeEx(Utility::Label("Lighting", FOLDER_ICON).c_str(), nodeFlags);
            
            if (ImGui::BeginPopupContextItem("LightingNode")) {

                if (ImGui::Checkbox("Directional Light 1", &m_model.m_enabledLights.at(0)))
                    lightStatusChanged(0, m_model.m_enabledLights.at(0));

                if (ImGui::Checkbox("Directional Light 2", &m_model.m_enabledLights.at(1)))
                    lightStatusChanged(1, m_model.m_enabledLights.at(1));

                if (ImGui::Checkbox("Directional Light 3", &m_model.m_enabledLights.at(2)))
                    lightStatusChanged(2, m_model.m_enabledLights.at(2));

                ImGui::EndPopup();
            }

            if (ImGui::IsItemClicked())
                select(SceneNode::Lighting);

            if (lightingNodeOpen) {

                const auto RenderLightNode = [this](std::uint8_t lightIndex, SceneNode nodeType) {
                    ImGui::BeginDisabled(!m_model.m_enabledLights.at(lightIndex)); {

                        const ImGuiTreeNodeFlags nodeFlags = NodeFlags(selected(nodeType), true);
                        ImGui::TreeNodeEx(Utility::Label("Directional Light " + std::to_string(lightIndex + 1), LIGHTBULB_ICON).c_str(), nodeFlags | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                        if (ImGui::IsItemClicked())
                            select(nodeType);

                        ImGui::EndDisabled();
                    }
                };

                RenderLightNode(0, SceneNode::Light1);
                RenderLightNode(1, SceneNode::Light2);
                RenderLightNode(2, SceneNode::Light3);

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::EndTabItem();
        ImGui::EndTabBar();
    }

    ImGui::End();

    if (openFileExplorer && !ImGui::IsPopupOpen(FileExplorer::kWindowId))
        ImGui::OpenPopup(FileExplorer::kWindowId);
}

void SceneTreeComponent::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel)
        return;

    for (std::size_t i = 0; i < m_model.m_enabledLights.size(); ++i)
        m_model.m_enabledLights.at(i) = pModel->m_lights.at(i)->enabled();

    m_model.m_modelName = pModel->m_modelName;
    m_model.m_modelLoaded = !pModel->m_pMesh->model()->empty();
    m_model.m_selectedMaterial = [pModel]() {
        if (dynamic_cast<LambertianMaterial*>(pModel->m_pMesh->material()))
            return 0;
    
        if (dynamic_cast<PhongMaterial*>(pModel->m_pMesh->material()))
            return 1;
    
        if (dynamic_cast<PhongTexturedMaterial*>(pModel->m_pMesh->material()))
            return 2;
    
        return 1;
    }();
}

const IComponent::DataModel* SceneTreeComponent::dataModel() const {
    return &m_model;
}

SceneTreeComponent::SceneNode SceneTreeComponent::selectedNode() const {

    for (std::size_t index = 0; index < m_selectionMask.size(); ++index) {
        if (m_selectionMask.test(index))
            return static_cast<SceneNode>(index);
    }

    return SceneNode::Scene;
}

bool SceneTreeComponent::selected(SceneNode sceneNode) const {
    return m_selectionMask.test(static_cast<uint8_t>(sceneNode));
}

void SceneTreeComponent::select(SceneNode sceneNode) {

    m_selectionMask.reset();
    m_selectionMask.flip(static_cast<uint8_t>(sceneNode));
    nodeSelected(sceneNode);
}
