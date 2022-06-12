#pragma once

#include "Light/ILight.hpp"

#include <memory>

#include <glm/vec3.hpp>

class ShaderProgram;

class DirectionalLight : public ILight {
public:
    DirectionalLight();
    virtual ~DirectionalLight() noexcept;

    DirectionalLight(const DirectionalLight& other);
    DirectionalLight& operator=(const DirectionalLight& other);

    DirectionalLight(DirectionalLight&& other) noexcept;
    DirectionalLight& operator=(DirectionalLight&& other) noexcept;
    
    virtual void apply(ShaderProgram* pShader) const override;

    void direction(const glm::vec3& value);
    void color(const glm::vec3& value);
    void intensity(float value);

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};