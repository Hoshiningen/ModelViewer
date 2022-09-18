#include "DirectoryTree.hpp"

#include <imgui.h>

void DirectoryTree::render() {

    if (ImGui::Begin(windowId())) {

        ImGui::End();
    }
}

const char* DirectoryTree::windowId() const {
    return "DirectoryTree";
}
