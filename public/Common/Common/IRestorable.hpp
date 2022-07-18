#pragma once

#include <array>
#include <algorithm>
#include <ranges>
#include <string_view>

#include <nlohmann/json.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class IRestorable {
public:
    virtual std::string_view id() const = 0;
    virtual nlohmann::json save() const = 0;
    virtual void restore(const nlohmann::json& settings) = 0;
};

namespace nlohmann {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec2, r, g)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec3, r, g, b)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec4, r, g, b, a)

template<std::size_t N>
struct adl_serializer<std::array<char, N>> {
    static void to_json(json& json, const std::array<char, N>& value) {
        json = std::string{ value.data() };
    }

    static void from_json(const json& json, std::array<char, N>& value) {
        const std::string temp = json.get<std::string>();
        std::ranges::copy(temp, value.begin());
    }
};
} // end nlohmann namespace
