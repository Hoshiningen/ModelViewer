#pragma once

#include <initializer_list>
#include <string>

struct Utility {
    static std::string Label(const char* label, const char* icon);
    static std::string Label(const std::string& label, const char* icon);

    static std::string Icon(const char* icon);

    static void Center(const char* text);
    static void Center(const std::string& text);

    static float ComputeRightAlignedCursorPos(const std::initializer_list<const char*>& widgetLabels);
    static float ComputeRightAlignedCursorPos(const std::initializer_list<float>& widgetWidths);
};
