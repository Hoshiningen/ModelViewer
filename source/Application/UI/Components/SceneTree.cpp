#include "UI/Components/SceneTree.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

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

    ImGui::Begin(windowId());

    if (ImGui::BeginTabBar("SceneTreeTabBar") && ImGui::BeginTabItem("Scene Tree")) {

        ImGuiTreeNodeFlags nodeFlags = NodeFlags(selected(SceneNode::Scene));
        if (ImGui::TreeNodeEx(Utility::Label("Scene", CUBES_ICON).c_str(), nodeFlags)) {
            if (ImGui::IsItemClicked())
                select(SceneNode::Scene);

            nodeFlags = NodeFlags(selected(SceneNode::Model));
            if (ImGui::TreeNodeEx(Utility::Label("Model", CUBE_ICON).c_str(), nodeFlags)) {
                if (ImGui::IsItemClicked())
                    select(SceneNode::Model);

                nodeFlags = NodeFlags(selected(SceneNode::Material));
                ImGui::TreeNodeEx(Utility::Label("Material", PALETTE_ICON).c_str(), nodeFlags | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                if (ImGui::IsItemClicked())
                    select(SceneNode::Material);

                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx(Utility::Label("Lighting", SUN_ICON).c_str(), NodeFlags(false))) {

                nodeFlags = NodeFlags(selected(SceneNode::Light1));
                ImGui::TreeNodeEx(Utility::Label("Directional Light 1", LIGHTBULB_ICON).c_str(), nodeFlags | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                if (ImGui::IsItemClicked())
                    select(SceneNode::Light1);

                nodeFlags = NodeFlags(selected(SceneNode::Light2));
                ImGui::TreeNodeEx(Utility::Label("Directional Light 2", LIGHTBULB_ICON).c_str(), nodeFlags | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                if (ImGui::IsItemClicked())
                    select(SceneNode::Light2);

                nodeFlags = NodeFlags(selected(SceneNode::Light3));
                ImGui::TreeNodeEx(Utility::Label("Directional Light 3", LIGHTBULB_ICON).c_str(), nodeFlags | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                if (ImGui::IsItemClicked())
                    select(SceneNode::Light3);

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
}

bool SceneTreeComponent::selected(SceneNode sceneNode) const {
    return m_selectionMask.test(static_cast<uint8_t>(sceneNode));
}

void SceneTreeComponent::select(SceneNode sceneNode) {

    m_selectionMask.reset();
    m_selectionMask.set(static_cast<uint8_t>(sceneNode), true);
    nodeSelected(sceneNode);
}
