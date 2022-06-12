#include "NonIndexedArtist.hpp"

#include "Geometry/VertexBuffered.hpp"
#include "Shader/ShaderProgram.hpp"

bool NonIndexedArtist::draw(const VertexBuffered& geometry, ShaderProgram*) {

    if (!validate(geometry))
        return false;

    if (geometry.indices().has_value())
        return false;

    if (bufferSize(geometry.vertexBufferId()) <= 0)
        initializeBufferData(geometry);

    glBindVertexArray(geometry.Id());
    glDrawArrays(GL_TRIANGLES, 0, geometry.vertices()->size());

    return true;
}

bool NonIndexedArtist::validate(const VertexBuffered& geometry) const {

    const auto vertices = geometry.vertices();
    const auto normals = geometry.normals();
    const auto colors = geometry.colors();

    if (!vertices.has_value() || !normals.has_value() || !colors.has_value())
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
    const GLsizeiptr colorBufferSize = sizeof(glm::vec4) * geometry.colors()->size();

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
    
    // Copy over the color data.
    glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBufferId());
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, geometry.colors()->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
