#include "Dialog.hpp"

Dialog::Dialog() { }

Dialog::Dialog(const std::string& title)
    : m_title(title) { }

Dialog::Dialog(const std::string& title, ImGuiWindowFlags flags)
    : m_title(title), m_windowFlags(flags) { }

Dialog::Dialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags)
    : m_title(title), m_position(position), m_windowFlags(flags) { }

Dialog::Dialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags)
    : m_title(title), m_position(position), m_size(size), m_windowFlags(flags) { }

void Dialog::render() {

    if (m_position)
        ImGui::SetNextWindowPos(*m_position);

    if (m_size)
        ImGui::SetNextWindowSize(*m_size);

    ImGui::Begin(m_title.c_str(), nullptr, m_windowFlags);
    defineUI();
    
    m_dialogActive = ImGui::IsAnyItemActive();

    ImGui::End();
}

void Dialog::position(const ImVec2& value) {
    m_position = value;
}

void Dialog::size(const ImVec2& value) {
    m_size = value;
}

void Dialog::title(const std::string& value) {
    m_title = value;
}

void Dialog::windowFlags(ImGuiWindowFlags value) {
    m_windowFlags = value;
}

bool Dialog::active() const {
    return m_dialogActive;
}
