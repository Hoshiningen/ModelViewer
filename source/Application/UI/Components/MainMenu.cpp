#include "Ui/Components/MainMenu.hpp"
#include "Ui/Components/MainFrame.hpp"
#include "UI/Components/ModelLoader.hpp"
#include "UI/Icons.hpp"
#include "UI/Utility.hpp"

#include <format>

#include <imgui.h>

void MainMenuComponent::render() {

    bool openModelLoader = false;
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem(Utility::Label("Open...", FOLDER_OPEN_ICON).c_str()))
                openModelLoader = true;

            if (ImGui::MenuItem("Close", nullptr, nullptr, m_model.m_modelLoaded))
                modelClosed();

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
                exited();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Properties")) {

            if (ImGui::MenuItem(Utility::Label("Scene...", CUBES_ICON).c_str()))
                scenePropertiesOpened();

            if (ImGui::MenuItem(Utility::Label("Lighting...", SUN_ICON).c_str()))
                lightPropertiesOpened();

            if (ImGui::MenuItem(Utility::Label("Model...", CUBE_ICON).c_str(), nullptr, nullptr, m_model.m_modelLoaded))
                modelPropertiesOpened();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Theme")) {

            if (ImGui::RadioButton("Classic", &m_model.m_selectedTheme, 0))
                themeChanged(0);

            if (ImGui::RadioButton("Light", &m_model.m_selectedTheme, 1))
                themeChanged(1);

            if (ImGui::RadioButton("Dark", &m_model.m_selectedTheme, 2))
                themeChanged(2);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (openModelLoader && !ImGui::IsPopupOpen(ModelLoaderComponent::kWindowId))
        ImGui::OpenPopup(ModelLoaderComponent::kWindowId);
}

void MainMenuComponent::syncFrom(const IComponent::DataModel* pFrom) {

    if (!pFrom)
        return;

    auto pModel = dynamic_cast<const MainFrameComponent::DataModel*>(pFrom);
    if (!pModel || !pModel->m_pMesh || !pModel->m_pWindowTheme)
        return;

    m_model.m_modelLoaded = !pModel->m_pMesh->model()->empty();
    m_model.m_selectedTheme = *pModel->m_pWindowTheme;
}
