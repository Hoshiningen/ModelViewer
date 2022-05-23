#pragma once

#include "VertexBuffered.hpp"

#include <memory>

#include <glm/vec3.hpp>

class Plane : public VertexBuffered {
public:
    Plane(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur);
    
    virtual ~Plane() noexcept;

    Plane(const Plane& other);
    Plane& operator=(const Plane& other);

    Plane(Plane&& other) noexcept;
    Plane& operator=(Plane&& other) noexcept;

    glm::vec3 normal() const;
    float width() const;
    float height() const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};