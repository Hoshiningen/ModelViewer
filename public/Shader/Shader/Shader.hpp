#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Shader final {
public:
    Shader();
    ~Shader() noexcept;

    Shader(const Shader& other);
    Shader& operator=(const Shader& other);

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    std::optional<GLuint> loadShader(const std::filesystem::path& path, GLenum shaderType);
    
    bool attachShader(GLuint shaderID) const;
    bool detachShader(GLuint shaderID) const;

    bool deleteShader(GLuint shaderID);
    bool deleteShaders();

    void createProgram();
    void deleteProgram();

    bool compileAndLink() const;
    void useProgram() const;

    void set(const std::string& variable, GLboolean value) const;
    void set(const std::string& variable, GLint value) const;
    void set(const std::string& variable, GLfloat value) const;
    void set(const std::string& variable, glm::vec3 value) const;
    void set(const std::string& variable, glm::vec4 value) const;
    void set(const std::string& variable, glm::mat4 value) const;

private:
    struct Private;
    std::unique_ptr<Private> m_pPrivate;
};
