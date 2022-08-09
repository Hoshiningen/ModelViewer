#include "Common/Math.hpp"
#include "Common/Constants.hpp"

#include "Geometry/VertexBuffered.hpp"

glm::vec3 ComputeCenter(const std::forward_list<VertexBuffered>* pModel) {

    glm::vec3 aabbCenter{ 0.f };
    if (!pModel)
        return aabbCenter;

    // Compute the bounding box around the model.
    const auto [min, max] = ComputeAABB(pModel);
    aabbCenter = (min + max) / 2.f;

    return aabbCenter;
}

float ComputeScale(const std::forward_list<VertexBuffered>* pModel, const glm::vec3 maxSize) {

    if (!pModel || (maxSize.x <= 0.f || maxSize.y <= 0.f || maxSize.z <= 0.f)) {
        assert(false);
        return 1.f;
    }

    const auto [min, max] = ComputeAABB(pModel);

    const float width = max.x - min.x;
    const float length = max.z - min.z;
    const float height = max.y - min.y;

    const float dWidth = width / maxSize.x;
    const float dLength = length / maxSize.z;
    const float dHeight = height / maxSize.y;

    const float maxDelta = std::max(dWidth, std::max(dLength, dHeight));

    return std::max(1.f / maxDelta, kMinScale);
}

std::pair<glm::vec3, glm::vec3> ComputeAABB(const std::forward_list<VertexBuffered>* pModel) {
    
    glm::vec3 min{ std::numeric_limits<float>::max() };
    glm::vec3 max{ std::numeric_limits<float>::lowest() };

    for (const VertexBuffered& buffer : *pModel) {

        const auto& vertices = buffer.vertices();
        if (!vertices)
            continue;

        for (const glm::vec3& vertex : *vertices) {

            min.x = std::min(min.x, vertex.x);
            min.y = std::min(min.y, vertex.y);
            min.z = std::min(min.z, vertex.z);

            max.x = std::max(max.x, vertex.x);
            max.y = std::max(max.y, vertex.y);
            max.z = std::max(max.z, vertex.z);
        }
    }

    return { min, max };
}

glm::vec3 RotateVector(const glm::vec3 vec, float pitchRad) {
    return MakeQuat(pitchRad) * glm::normalize(vec);
}

glm::vec3 RotateVector(const glm::vec3 vec, float pitchRad, float yawRad, bool localQuat) {
    return MakeQuat(pitchRad, yawRad, localQuat) * glm::normalize(vec);
}

glm::vec3 RotateVector(const glm::vec3 vec, float pitchRad, float yawRad, float rollRad, bool localQuat) {
    return MakeQuat(pitchRad, yawRad, rollRad, localQuat) * glm::normalize(vec);
}

glm::quat MakeQuat(float pitchRad) {
    return glm::angleAxis(pitchRad, kWorldRight);
}

glm::quat MakeQuat(float pitchRad, float yawRad, bool localQuat) {

    const glm::quat pitchQuat = MakeQuat(pitchRad);
    const glm::vec3 rotUp = glm::normalize(pitchQuat * kWorldUp);
    const glm::quat yawQuat = glm::angleAxis(yawRad, localQuat ? rotUp : kWorldUp);

    return yawQuat * pitchQuat;
}

glm::quat MakeQuat(float pitchRad, float yawRad, float rollRad, bool localQuat) {

    const glm::quat pitchYawQuat = MakeQuat(pitchRad, yawRad, localQuat);
    const glm::vec3 rotForward = glm::normalize(pitchYawQuat * kWorldForward);
    const glm::quat rollQuat = glm::angleAxis(rollRad, localQuat ? rotForward : kWorldForward);

    return rollQuat * pitchYawQuat;
}
