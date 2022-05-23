#pragma once

#include "Camera/Camera.hpp"

#include <memory>

#include <glm/vec3.hpp>

class PerspectiveCamera;

class OrthographicCamera : public Camera {
public:
    OrthographicCamera();
    OrthographicCamera(float left, float right, float bottom, float top, float nearZ, float farZ);
    OrthographicCamera(const glm::vec3& position, float aspectRatio, float fovY, float nearZ, float farZ);
    OrthographicCamera(const PerspectiveCamera& camera);
    
    virtual ~OrthographicCamera() noexcept;

    OrthographicCamera(const OrthographicCamera& other);
    OrthographicCamera& operator=(const OrthographicCamera& other);

    OrthographicCamera(OrthographicCamera&& other) noexcept;
    OrthographicCamera& operator=(OrthographicCamera&& other) noexcept;

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

protected:
    virtual glm::mat4 projection() const override;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
