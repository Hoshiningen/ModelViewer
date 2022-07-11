#pragma once

#include <optional>
#include <string>

#include <imgui.h>

class Dialog {
public:
    Dialog();
    Dialog(const std::string& title);
    Dialog(const std::string& title, ImGuiWindowFlags flags);
    Dialog(const std::string& title, const ImVec2& position, ImGuiWindowFlags flags);
    Dialog(const std::string& title, const ImVec2& position, const ImVec2& size, ImGuiWindowFlags flags);

    virtual void render() final;

    void position(const ImVec2& value);
    void size(const ImVec2& value);
    void title(const std::string& value);
    void windowFlags(ImGuiWindowFlags value);

    bool active() const;

protected:
    static constexpr std::size_t kTextBufferSize = 256;

    virtual void defineUI() = 0;
    virtual void initializeUI() {}

    std::string m_title;
    std::optional<ImVec2> m_position;
    std::optional<ImVec2> m_size;
    ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_None;

private:
    bool m_dialogActive = false;
};
