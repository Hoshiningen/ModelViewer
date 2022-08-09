#pragma once

#include <cstdint>

#include <glm/vec3.hpp>

constexpr uint8_t kMaxLights = 3;
static_assert(kMaxLights > 0, "There must be at least one light present in the scene.");

constexpr glm::vec3 kWorldRight{ 1.f, 0.f, 0.f};
constexpr glm::vec3 kWorldUp{ 0.f, 1.f, 0.f };
constexpr glm::vec3 kWorldForward{ 0.f, 0.f, 1.f };

constexpr glm::vec3 kMaxModelSize{ 2.f };

constexpr float kMinPitch = -90.f;
constexpr float kMaxPitch = 90.f;

constexpr float kMinScale = 0.001f;
