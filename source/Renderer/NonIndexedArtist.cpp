#include "NonIndexedArtist.hpp"

#include "Geometry/VertexBuffered.hpp"
#include "Shader/Shader.hpp"

namespace {
constexpr glm::vec3 kLightPosition{ 21.f, 7.f, 7.f };
constexpr glm::vec3 kAmbient{ 1.f, 1.f, 1.f };
constexpr glm::vec3 kDiffuse{ 1.f, 0.89f, 0.52f };
constexpr glm::vec3 kSpecular{ 1.f, 1.f, 1.f };

constexpr float kAmbientIntensity = 0.1f;
constexpr float kSpecularIntensity = 1.0f;
constexpr float kSpecularShininess = 128.f;
} // end unnamed namespace

NonIndexedArtist::NonIndexedArtist(Shader* pShader)
    : GeometryArtist(pShader) {}

bool NonIndexedArtist::draw(const VertexBuffered& geometry, const glm::vec4& color) {

    if (!m_pShader || !validate(geometry))
        return false;

    if (geometry.indices().has_value())
        return false;

    m_pShader->set("objectColor", glm::vec3{color});
    m_pShader->set("ambientIntensity", kAmbientIntensity);
    m_pShader->set("specularIntensity", kSpecularIntensity);
    m_pShader->set("specularShininess", kSpecularShininess);

    m_pShader->set("directionalLight.direction", glm::normalize(-kLightPosition));
    m_pShader->set("directionalLight.ambient", kAmbient);
    m_pShader->set("directionalLight.diffuse", kDiffuse);
    m_pShader->set("directionalLight.specular", kSpecular);

    if (bufferSize(geometry.vertexBufferId()) <= 0)
        initializeBufferData(geometry);

    glBindVertexArray(geometry.Id());
    glDrawArrays(GL_TRIANGLES, 0, geometry.vertices()->size());

    return true;
}

bool NonIndexedArtist::validate(const VertexBuffered& geometry) const {

    const auto vertices = geometry.vertices();
    const auto normals = geometry.normals();

    if (!vertices.has_value() || !normals.has_value())
        return false;

    if (vertices->size() != normals->size())
        return false;

    if (vertices->size() % 3 != 0)
        return false;

    return true;
}

void NonIndexedArtist::initializeBufferData(const VertexBuffered& geometry) const {

    const GLsizeiptr vertexBufferSize = sizeof(glm::vec3) * geometry.vertices()->size();
    const GLsizeiptr normalBufferSize = sizeof(glm::vec3) * geometry.normals()->size();

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
    
    glBindVertexArray(0);
}
