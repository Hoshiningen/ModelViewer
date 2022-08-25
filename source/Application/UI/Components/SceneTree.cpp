#include "UI/Components/SceneTree.hpp"
#include "UI/Components/ModelLoader.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <format>
#include <iostream>

#include <imgui.h>

ImGuiTreeNodeFlags NodeFlags(bool selected) {
    static const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf;
    return selected ? nodeFlags | ImGuiTreeNodeFlags_Selected : nodeFlags;
}

const char* SceneTreeComponent::windowId() const {
    return "SceneTree";
}

void SceneTreeComponent::render() {

    bool openModelLoader = false;
    ImGui::Begin(windowId());

    if (ImGui::BeginTabBar("SceneTreeTabBar") && ImGui::BeginTabItem("Scene Tree")) {

        ImGuiTreeNodeFlags nodeFlags = NodeFlags(selected(SceneNode::Scene));
        if (ImGui::TreeNodeEx(Utility::Label("Scene", CUBES_ICON).c_str(), nodeFlags)) {
            if (ImGui::IsItemClicked())
                select(SceneNode::Scene);

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Load Model..."))
                    openModelLoader = true;

                ImGui::EndPopup();
            }

            nodeFlags = NodeFlags(selected(SceneNode::Model));
            if (ImGui::TreeNodeEx(Utility::Label("Model", CUBE_ICON).c_str(), nodeFlags)) {
                if (ImGui::IsItemClicked())
                    select(SceneNode::Model);

                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::BeginMenu("Material")) {

                        if (ImGui::RadioButton("Lambertian", &m_selectedMaterial, 0))
                            materialSelected(m_selectedMaterial, selectedNode());

                        if (ImGui::RadioButton("Phong", &m_selectedMaterial, 1))
                            materialSelected(m_selectedMaterial, selectedNode());

                        if (ImGui::RadioButton("Phong Textured", &m_selectedMaterial, 2))
                            materialSelected(m_selectedMaterial, selectedNode());

                        ImGui::EndMenu();
                    }

                    ImGui::EndPopup();
                }

                const char* materialName = [this] {
                    switch (m_selectedMaterial) {
                    case 0: return "Lambertian Material";
                    case 1: return "Phong Material";
                    case 2: return "Phong Textured Material";
                    default: return "Material";
                    }
                }();

                nodeFlags = NodeFlags(selected(SceneNode::Material));
                ImGui::TreeNodeEx(Utility::Label(materialName, PALETTE_ICON).c_str(), nodeFlags | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                if (ImGui::IsItemClicked())
                    select(SceneNode::Material);

                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx(Utility::Label("Lighting", SUN_ICON).c_str(), NodeFlags(false))) {

                if (ImGui::BeginPopupContextItem()) {
                
                    if (ImGui::Checkbox("Directional Light 1", &m_enabledLights.at(0)))
                        lightStatusChanged(0, m_enabledLights.at(0));
                
                    if (ImGui::Checkbox("Directional Light 2", &m_enabledLights.at(1)))
                        lightStatusChanged(1, m_enabledLights.at(1));
                
                    if (ImGui::Checkbox("Directional Light 3", &m_enabledLights.at(2)))
                        lightStatusChanged(2, m_enabledLights.at(2));
                
                    ImGui::EndPopup();
                }

                const auto RenderLightNode = [this](std::uint8_t lightIndex, SceneNode nodeType) {
                    ImGui::BeginDisabled(!m_enabledLights.at(lightIndex)); {

                        const ImGuiTreeNodeFlags nodeFlags = NodeFlags(selected(nodeType));
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

            nodeFlags = NodeFlags(selected(SceneNode::Camera));
            ImGui::TreeNodeEx(Utility::Label("Camera", CAMERA_ICON).c_str(), nodeFlags);
            if (ImGui::IsItemClicked())
                select(SceneNode::Camera);

            ImGui::TreePop();
        }

        ImGui::EndTabItem();
        ImGui::EndTabBar();
    }

    ImGui::End();

    if (openModelLoader && !ImGui::IsPopupOpen(ModelLoaderComponent::kWindowId)) {
        ImGui::OpenPopup(ModelLoaderComponent::kWindowId);
    }
}

SceneTreeComponent::SceneNode SceneTreeComponent::selectedNode() const {

    for (std::size_t index = 0; index < m_selectionMask.size(); ++index) {
        if (m_selectionMask.test(index))
            return static_cast<SceneNode>(index);
    }
}

bool SceneTreeComponent::selected(SceneNode sceneNode) const {
    return m_selectionMask.test(static_cast<uint8_t>(sceneNode));
}

void SceneTreeComponent::select(SceneNode sceneNode) {

    m_selectionMask.reset();
    m_selectionMask.flip(static_cast<uint8_t>(sceneNode));
    nodeSelected(sceneNode);
}
