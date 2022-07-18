#pragma once

#include "Camera/Camera.hpp"

#include "Common/ClassMacros.hpp"

#include <glm/vec3.hpp>

class PerspectiveCamera;

class OrthographicCamera : public Camera {
public:
    OrthographicCamera();
    OrthographicCamera(float left, float right, float bottom, float top, float nearZ, float farZ);
    OrthographicCamera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ);
    OrthographicCamera(const PerspectiveCamera& camera);

    DECLARE_GETTER_IMMUTABLE(leftExtent, float)
    DECLARE_SETTER_COPY(leftExtent, float)

    DECLARE_GETTER_IMMUTABLE(rightExtent, float)
    DECLARE_SETTER_COPY(rightExtent, float)
    
    DECLARE_GETTER_IMMUTABLE(bottomExtent, float)
    DECLARE_SETTER_COPY(bottomExtent, float)
    
    DECLARE_GETTER_IMMUTABLE(topExtent, float)
    DECLARE_SETTER_COPY(topExtent, float)

    virtual float fovY() const override;
    virtual void fovY(float value) override;

    virtual float aspectRatio() const override;
    virtual void aspectRatio(float value) override;

protected:
    virtual glm::mat4 projection() const override;

private:
    COMPILATION_FIREWALL_COPY_MOVE(OrthographicCamera)
};
