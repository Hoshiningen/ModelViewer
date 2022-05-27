#include "IndexedArtist.hpp"

#include "Geometry/VertexBuffered.hpp"
#include "Shader/Shader.hpp"

#include <algorithm>
#include <ranges>

namespace {
constexpr glm::vec3 kLightPosition{ 21.f, 7.f, 7.f };
constexpr glm::vec3 kAmbient{ 1.f, 1.f, 1.f };
constexpr glm::vec3 kDiffuse{ 1.f, 0.89f, 0.52f };
constexpr glm::vec3 kSpecular{ 1.f, 1.f, 1.f };

constexpr float kAmbientIntensity = 0.1f;
constexpr float kSpecularIntensity = 1.0f;
constexpr float kSpecularShininess = 128.f;
} // end unnamed namespace

IndexedArtist::IndexedArtist(Shader* pShader)
    : GeometryArtist(pShader) {}

bool IndexedArtist::draw(const VertexBuffered& geometry, const glm::vec4& color) {

    if (!m_pShader || !validate(geometry))
        return false;

    m_pShader->set("objectColor", glm::vec3{ color });
    m_pShader->set("ambientIntensity", kAmbientIntensity);
    m_pShader->set("specularIntensity", kSpecularIntensity);
    m_pShader->set("specularShininess", kSpecularShininess);

    m_pShader->set("directionalLight.direction", glm::normalize(-kLightPosition));
    m_pShader->set("directionalLight.ambient", kAmbient);
    m_pShader->set("directionalLight.diffuse", kDiffuse);
    m_pShader->set("directionalLight.specular", kSpecular);

    // Only copy data once for this object.
    if (bufferSize(geometry.vertexBufferId()) <= 0)
        initializeBufferData(geometry);

    glBindVertexArray(geometry.Id());
    glDrawElements(GL_TRIANGLES, geometry.indices()->size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

    return true;
}

bool IndexedArtist::validate(const VertexBuffered& geometry) const {

    if (!geometry.initialized())
        return false;

    const auto vertices = geometry.vertices();
    const auto normals = geometry.normals();
    const auto indices = geometry.indices();

    if (!vertices.has_value() || !normals.has_value() || !indices.has_value())
        return false;

    if (vertices->size() != normals->size())
        return false;

    const auto maxIndex = std::ranges::max_element(*indices);
    if (*maxIndex >= vertices->size())
        return false;

    return true;
}

void IndexedArtist::initializeBufferData(const VertexBuffered& geometry) const {

    const GLsizeiptr vertexBufferSize = sizeof(glm::vec3) * geometry.vertices()->size();
    const GLsizeiptr normalBufferSize = sizeof(glm::vec3) * geometry.normals()->size();
    const GLsizeiptr indicesBufferSize = sizeof(uint32_t) * geometry.indices()->size();

    glBindVertexArray(geometry.Id());

    // Copy over the vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferId());
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, geometry.vertices()->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // Copy over the normal data.
    glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBufferId());
    glBufferData(GL_ARRAY_BUFFER, normalBufferSize, geometry.normals()->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);

    // Copy over the index data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.indexBufferId());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBufferSize, geometry.indices()->data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}
