#pragma once

#include <cstdint>

constexpr uint8_t kMaxLights = 3;
static_assert(kMaxLights > 0, "There must be at least one light present in the scene.");
