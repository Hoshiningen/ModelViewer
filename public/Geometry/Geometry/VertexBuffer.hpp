#pragma once

#include "Common/ClassMacros.hpp"

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class VertexBuffer {
public:
    VertexBuffer();

    void addColor(const glm::vec4& color);
    void addIndex(uint32_t index);
    void addNormal(const glm::vec3& normal);
    void addTexel(const glm::vec2& texel);
    void addVertex(const glm::vec3& vertex);

    DECLARE_GETTER_CONST_CORRECT(colors, std::vector<glm::vec4>)
    DECLARE_GETTER_CONST_CORRECT(indices, std::vector<uint32_t>)
    DECLARE_GETTER_CONST_CORRECT(normals, std::vector<glm::vec3>)
    DECLARE_GETTER_CONST_CORRECT(texels, std::vector<glm::vec2>)
    DECLARE_GETTER_CONST_CORRECT(vertices, std::vector<glm::vec3>)
    
private:
    COMPILATION_FIREWALL_COPY_MOVE(VertexBuffer)
};
