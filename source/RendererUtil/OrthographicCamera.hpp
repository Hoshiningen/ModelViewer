#pragma once

#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PerspectiveCamera;
class Renderer;

class OrthographicCamera : public Camera {
public:
    OrthographicCamera() = default;
    OrthographicCamera(float left, float right, float bottom, float top, float nearZ, float farZ);
    OrthographicCamera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ);
    OrthographicCamera(const PerspectiveCamera& camera);

    float leftExtent() const;
    void leftExtent(float value);

    float rightExtent() const;
    void rightExtent(float value);
    
    float bottomExtent() const;
    void bottomExtent(float value);
    
    float topExtent() const;
    void topExtent(float value);

    virtual float fovY() const override;
    virtual void fovY(float value) override;

    virtual float aspectRatio() const override;
    virtual void aspectRatio(float value) override;

    virtual std::vector<Geometry> debugFrustum() const override;

protected:
    virtual glm::mat4 projection() const override;

private:
    void computeExtents();

    float m_leftExtent = -0.5f;
    float m_rightExtent = 0.5f;
    float m_bottomExtent = -0.5f;
    float m_topExtent = 0.5f;
};
