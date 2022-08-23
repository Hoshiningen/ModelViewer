#pragma once

#include "Common/ClassMacros.hpp"

#include <forward_list>
#include <vector>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Camera;
class DirectionalLight;
class IMaterial;
class Mesh;
class ShaderCache;
class Texture;
class VertexBuffered;

class Renderer {
public:
    static void Allocate(Mesh& mesh);
    static void Allocate(const Texture& texture, std::uint8_t* pData);

    static void Configure(Mesh& mesh);
    static void Configure(Texture& texture);

    Renderer();

    void createFramebuffer(const glm::uvec2& dimensions);
    void purgeFramebuffer();

    DECLARE_GETTER_IMMUTABLE_COPY(framebufferId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(framebufferTextureId, GLuint)
    DECLARE_GETTER_IMMUTABLE_COPY(framebufferBitplane, GLbitfield)

    DECLARE_SETTER_CONSTREF(directionalLights, std::vector<DirectionalLight*>)

    void setup();
    void camera(Camera* pCamera);

    void draw(const Mesh& mesh) const;

    void ambientColor(glm::vec3* pAmbientColor, float* pAmbientIntensity);

private:
    static ShaderCache* shaderCache();

    COMPILATION_FIREWALL(Renderer)
};
