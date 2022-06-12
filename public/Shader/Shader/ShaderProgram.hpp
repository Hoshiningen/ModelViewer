#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class ShaderProgram final {
public:
    ShaderProgram();
    ~ShaderProgram() noexcept;

    ShaderProgram(const ShaderProgram& other);
    ShaderProgram& operator=(const ShaderProgram& other);

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

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

    GLuint id() const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
