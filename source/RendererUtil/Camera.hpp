#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera() = default;
    Camera(float nearZ, float farZ)
        : m_nearZ(nearZ), m_farZ(farZ) {}

    glm::vec3 position() const { return m_position; }
    void position(const glm::vec3& value) { m_position = value; }

    float near() const { return m_nearZ; }
    void near(float value) { m_nearZ = value; }

    float far() const { return m_farZ; }
    void far(float value) { m_farZ = value; }

    glm::mat4 viewProjection() const {
        return m_viewProjection;
    }

    void update() {
        m_viewProjection = projection() * view();
    }

protected:
    virtual glm::mat4 projection() const {
        return glm::identity<glm::mat4>();
    };

    float m_nearZ = 0.1f;
    float m_farZ = 100.f;

    glm::mat4 m_viewProjection;

private:
    glm::mat4 view() const {
        return glm::lookAt(m_position, m_kCenter, m_kUp);
    }

    const glm::vec3 m_kCenter{ 0.f, 0.f, 0.f };
    const glm::vec3 m_kUp{ 0.f, 1.f, 0.f };
    
    glm::vec3 m_position{ 0.f, 0.f, 0.f };
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera() = default;
    PerspectiveCamera(float aspectRatio, float fov, float nearZ, float farZ)
        : Camera(nearZ, farZ), m_aspectRatio(aspectRatio), m_fov(fov) {}

    float fov() const { return m_fov; }
    void fov(float value) { m_fov = value; }

    float aspectRatio() const { return m_aspectRatio; }
    void aspectRatio(float value) { m_aspectRatio = value; }

protected:
    virtual glm::mat4 projection() const override {
        return glm::perspective(m_fov, m_aspectRatio, m_nearZ, m_farZ);
    }

private:
    float m_aspectRatio = 1.f;
    float m_fov = 70.0;
};

class OrthographicCamera : public Camera {
public:
    OrthographicCamera() = default;
    OrthographicCamera(float left, float right, float bottom, float top, float nearZ, float farZ)
        : Camera(nearZ, farZ), m_left(left), m_right(right), m_bottom(bottom), m_top(top) {}

    float left() const { return m_left; }
    void left(float value) { m_left = value; }

    float right() const { return m_right; }
    void right(float value) { m_right = value; }
    
    float bottom() const { return m_bottom; }
    void bottom(float value) { m_bottom = value; }
    
    float top() const { return m_top; }
    void top(float value) { m_top = value; }

protected:
    virtual glm::mat4 projection() const override {
        return glm::ortho(m_left, m_right, m_bottom, m_top, m_nearZ, m_farZ);
    }

private:
    float m_left = 0.f;
    float m_right = 0.f;
    float m_bottom = 0.f;
    float m_top = 0.f;
};