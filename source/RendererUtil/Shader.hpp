#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_set>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader final {
public:
    ~Shader() {
        deleteShaders();
        glDeleteProgram(m_program);
    }

    std::optional<GLuint> loadShader(const std::filesystem::path& path, GLenum shaderType);
    
    bool attachShader(GLuint shaderID) const;
    bool detachShader(GLuint shaderID) const;

    bool deleteShader(GLuint shaderID);
    bool deleteShaders();

    void createProgram() { m_program = glCreateProgram(); }

    bool compileAndLink() const;
    void useProgram() const;

    void set(const std::string& variable, GLboolean value) const;
    void set(const std::string& variable, GLint value) const;
    void set(const std::string& variable, GLfloat value) const;
    void set(const std::string& variable, glm::vec3 value) const;
    void set(const std::string& variable, glm::vec4 value) const;
    void set(const std::string& variable, glm::mat4 value) const;

private:
    bool reportError(GLuint objectID) const;
    bool isProgramLinked() const;

    GLuint m_program = 0;
    std::unordered_set<GLuint> m_shaders;
};
