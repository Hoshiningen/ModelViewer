#pragma once

#include "Common/ClassMacros.hpp"

#include <forward_list>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Camera;
class DirectionalLight;
class IMaterial;
class Mesh;
class ShaderProgram;
class Texture;
class VertexBuffered;

class Renderer {
public:
    static void Allocate(const Texture& texture, std::uint8_t* pData);
    static void Create(Texture& texture);

    Renderer();

    void createFramebuffer(const glm::uvec2& dimensions);
    void resetFramebuffer();

    DECLARE_GETTER_IMMUTABLE_COPY(framebufferId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(framebufferTextureId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(framebufferBitplane, GLbitfield)

    void setup();
    void camera(Camera* pCamera);

    void draw(const Mesh& mesh) const;

    void initializeMesh(Mesh& mesh) const;

    void directionalLight(DirectionalLight** ppLight, uint8_t lightIndex);
    void ambientColor(glm::vec3* pAmbientColor, float* pAmbientIntensity);

private:
    COMPILATION_FIREWALL(Renderer)
};
