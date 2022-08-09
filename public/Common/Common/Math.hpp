#pragma once

#include <forward_list>
#include <utility>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

class VertexBuffered;

glm::vec3 ComputeCenter(const std::forward_list<VertexBuffered>* pModel);
float ComputeScale(const std::forward_list<VertexBuffered>* pModel, const glm::vec3 maxSize);

std::pair<glm::vec3, glm::vec3> ComputeAABB(const std::forward_list<VertexBuffered>* pModel);

glm::vec3 RotateVector(const glm::vec3 vec, float pitchRad);
glm::vec3 RotateVector(const glm::vec3 vec, float pitchRad, float yawRad, bool localQuat);
glm::vec3 RotateVector(const glm::vec3 vec, float pitchRad, float yawRad, float rollRad, bool localQuat);

glm::quat MakeQuat(float pitchRad);
glm::quat MakeQuat(float pitchRad, float yawRad, bool localQuat);
glm::quat MakeQuat(float pitchRad, float yawRad, float rollRad, bool localQuat);
