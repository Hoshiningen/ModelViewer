#include "Shader.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

std::optional<GLuint> Shader::loadShader(const std::filesystem::path& path, GLenum shaderType) {

    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path)) {
        std::cerr << "Error: Could not load the shader at: " << path << "\n";
        return {};
    }

    std::ifstream shaderFile{ path };
    if (!shaderFile.is_open()) {
        std::cerr << "Error: Unable to open a file handle to the shader at: " << path << "\n";
        return {};
    }

    GLuint shaderID = glCreateShader(shaderType);
    if (!shaderID) {
        std::cerr << "Error: Failed to create the shader with the given type\n";
        return {};
    }

    // Load the file data into a c-string
    std::stringstream ss;
    ss << shaderFile.rdbuf();
    std::string temp = ss.str();
    const char* source = temp.c_str();

    // Load the shader source into the generated shader object represented by shaderID
    glShaderSource(shaderID, 1, &source, nullptr);

    // Store in our list of shaders
    m_shaders.emplace(shaderID);

    return shaderID;
}

bool Shader::attachShader(GLuint shaderID) const {

    if (glIsShader(shaderID) == GL_FALSE)
        return false;

    glAttachShader(m_program, shaderID);
    if (glGetError() == GL_INVALID_OPERATION) {
        std::cerr << "Error: Shader has already been attached to the given program\n";
        return false;
    }

    return true;
}

bool Shader::detachShader(GLuint shaderID) const {

    if (glIsShader(shaderID) == GL_FALSE)
        return false;

    glDetachShader(m_program, shaderID);
    if (glGetError() == GL_INVALID_OPERATION) {
        std::cerr << "Error: Shader has not been attached to the given program\n";
        return false;
    }

    return true;
}

bool Shader::deleteShader(GLuint shaderID) {

    if (glIsShader(shaderID) == GL_FALSE)
        return false;

    GLint isDeleted = 0;
    glGetShaderiv(shaderID, GL_DELETE_STATUS, &isDeleted);

    if (isDeleted == GL_TRUE)
        return false;

    glDeleteShader(shaderID);

    return true;
}

bool Shader::deleteShaders() {

    bool allDeleted = true;

    for (GLuint shader : m_shaders)
        allDeleted &= deleteShader(shader);

    return allDeleted;
}

bool Shader::compileAndLink() const {

    GLsizei numAttached = 0;

    std::vector<GLuint> attachedShaders;
    attachedShaders.resize(20);

    glGetAttachedShaders(m_program, static_cast<GLsizei>(attachedShaders.size()), &numAttached, attachedShaders.data());
    attachedShaders.resize(numAttached);

    // Bail early if there were no shaders attached
    if (!numAttached) {
        std::cerr << "Error: Cannot use a program that has no attached shaders\n";
        return false;
    }

    for (GLuint shader : attachedShaders) {
        glCompileShader(shader);
        if (!reportError(shader))
            return false;
    }

    glLinkProgram(m_program);
    if (!reportError(m_program))
        return false;

    return true;
}

void Shader::useProgram() const {
    glUseProgram(m_program);
}

void Shader::set(const std::string& variable, GLboolean value) const {

    if (!isProgramLinked()) {
        std::cerr << "Error: Program isn't linked. Can't access uniform variables.\n";
        return;
    }

    glUniform1i(glGetUniformLocation(m_program, variable.c_str()), static_cast<GLint>(value));
}

void Shader::set(const std::string& variable, GLint value) const {

    if (!isProgramLinked()) {
        std::cerr << "Error: Program isn't linked. Can't access uniform variables.\n";
        return;
    }

    glUniform1i(glGetUniformLocation(m_program, variable.c_str()), value);
}

void Shader::set(const std::string& variable, GLfloat value) const {

    if (!isProgramLinked()) {
        std::cerr << "Error: Program isn't linked. Can't access uniform variables.\n";
        return;
    }

    glUniform1f(glGetUniformLocation(m_program, variable.c_str()), value);
}

void Shader::set(const std::string& variable, glm::vec3 value) const {

    if (!isProgramLinked()) {
        std::cerr << "Error: Program isn't linked. Can't access uniform variables.\n";
        return;
    }

    glUniform3f(glGetUniformLocation(m_program, variable.c_str()), value.x, value.y, value.z);
}

void Shader::set(const std::string& variable, glm::vec4 value) const
{
    if (!isProgramLinked()) {
        std::cerr << "Error: Program isn't linked. Can't access uniform variables.\n";
        return;
    }

    glUniform4f(glGetUniformLocation(m_program, variable.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::set(const std::string& variable, glm::mat4 value) const {

    if (!isProgramLinked()) {
        std::cerr << "Error: Program isn't linked. Can't access uniform variables.\n";
        return;
    }

    glUniformMatrix4fv(glGetUniformLocation(m_program, variable.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

bool Shader::reportError(GLuint objectID) const {

    // Buffer to contain the error information
    std::array<char, 512> infoLog;

    if (glIsProgram(objectID) == GL_TRUE) {

        GLint success = 0;
        glGetProgramiv(objectID, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(objectID, sizeof(infoLog), nullptr, infoLog.data());
            std::cerr << "Shader Link Error: " << infoLog.data() << "\n";
        }

        return success == GL_TRUE;
    }

    if (glIsShader(objectID) == GL_TRUE) {

        GLint success = 0;
        glGetShaderiv(objectID, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(objectID, sizeof(infoLog), nullptr, infoLog.data());
            std::cerr << "Shader Compilation Error: " << infoLog.data() << "\n";
        }

        return success == GL_TRUE;
    }

    return false;
}

bool Shader::isProgramLinked() const {

    GLint isLinked = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);

    return isLinked;;
}
