#pragma once

#include "Common/ClassMacros.hpp"

#include <filesystem>
#include <optional>
#include <string_view>
#include <unordered_set>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class ShaderProgram final {
public:
    ShaderProgram();

    std::optional<GLuint> loadShader(const std::filesystem::path& path, GLenum ShaderProgramType);
    
    bool attachShader(GLuint ShaderProgramID) const;
    bool detachShader(GLuint ShaderProgramID) const;

    bool destroyShader(GLuint ShaderProgramID);
    bool destroyShaders();

    void create();
    void destroy();

    bool compileAndLink() const;
    void use() const;

    void set(const std::string& variable, GLboolean value) const;
    void set(const std::string& variable, GLint value) const;
    void set(const std::string& variable, GLfloat value) const;
    void set(const std::string& variable, glm::vec3 value) const;
    void set(const std::string& variable, glm::vec4 value) const;
    void set(const std::string& variable, glm::mat4 value) const;

    DECLARE_GETTER_IMMUTABLE_COPY(id, GLuint)

    DECLARE_GETTER_IMMUTABLE_COPY(attributes, std::unordered_set<std::string_view>)
    DECLARE_GETTER_IMMUTABLE_COPY(uniforms, std::unordered_set<std::string_view>)

    bool hasAttribute(const std::string& name) const;
    bool hasUniform(const std::string& name) const;

    std::optional<GLuint> attributeLocation(const std::string& attributeName) const;

private:
    COMPILATION_FIREWALL_COPY_MOVE(ShaderProgram)
};
