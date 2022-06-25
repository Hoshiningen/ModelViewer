#include "Shader/ShaderProgram.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace {
template<typename...Handlers>
struct Visitor : public Handlers...{
    using Handlers::operator()...;
};
} // end unnamed namespace

struct ShaderProgram::Private {
    bool reportError(GLuint objectID) const;
    bool isProgramLinked() const;

    void readAttributes();
    void readUniforms();

    using Variable = std::variant<GLboolean, GLint, GLfloat, glm::vec3, glm::vec4, glm::mat4>;
    void set(const std::string& variable, Variable value);

    GLuint m_program = 0;
    std::unordered_set<GLuint> m_shaders;
    std::unordered_set<std::string> m_uniforms;
    std::unordered_map<std::string, GLuint> m_attributes;
};

bool ShaderProgram::Private::reportError(GLuint objectID) const {

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

bool ShaderProgram::Private::isProgramLinked() const {

    GLint isLinked = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);

    return isLinked;
}

void ShaderProgram::Private::readAttributes() {

    if (!isProgramLinked())
        return;

    GLint numAttributes = 0;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    GLint bufferSize = 0;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufferSize);

    for (GLint attributeIndex = 0; attributeIndex < numAttributes; ++attributeIndex) {

        std::vector<GLchar> name;
        name.resize(bufferSize);

        GLint size = 0;
        GLenum type = 0;
        GLsizei nameLength = 0;
        glGetActiveAttrib(m_program, attributeIndex, bufferSize, &nameLength, &size, &type, name.data());

        const GLint attributeLocation = glGetAttribLocation(m_program, name.data());
        m_attributes.emplace(std::string{ name.data(), static_cast<std::size_t>(nameLength) }, attributeLocation);
    }
}

void ShaderProgram::Private::readUniforms() {

    if (!isProgramLinked())
        return;

    GLint numUniforms = 0;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &numUniforms);

    GLint bufferSize = 0;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufferSize);

    for (GLint uniformIndex = 0; uniformIndex < numUniforms; ++uniformIndex) {

        std::vector<GLchar> name;
        name.resize(bufferSize);

        GLint size = 0;
        GLenum type = 0;
        GLsizei nameLength = 0;
        glGetActiveUniform(m_program, uniformIndex, bufferSize, &nameLength, &size, &type, name.data());

        m_uniforms.emplace(name.data(), nameLength);
    }
}

void ShaderProgram::Private::set(const std::string& variable, Variable value) {

    if (!isProgramLinked()) {
        std::cerr << "Error: Program isn't linked. Unable to set '" << variable << "'.\n";
        return;
    }

    std::visit(Visitor{
        [this, &variable](GLboolean value) {
            glUniform1i(glGetUniformLocation(m_program, variable.c_str()), static_cast<GLint>(value));
        },
        [this, &variable](GLint value) {
            glUniform1i(glGetUniformLocation(m_program, variable.c_str()), value);
        },
        [this, &variable](GLfloat value) {
            glUniform1f(glGetUniformLocation(m_program, variable.c_str()), value);
        },
        [this, &variable](glm::vec3 value) {
            glUniform3f(glGetUniformLocation(m_program, variable.c_str()), value.x, value.y, value.z);
        },
        [this, &variable](glm::vec4 value) {
            glUniform4f(glGetUniformLocation(m_program, variable.c_str()), value.x, value.y, value.z, value.w);
        },
        [this, &variable](glm::mat4 value) {
            glUniformMatrix4fv(glGetUniformLocation(m_program, variable.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        }
    }, value);
}


ShaderProgram::ShaderProgram()
    : m_pPrivate(std::make_unique<Private>()) {}

ShaderProgram::~ShaderProgram() {
    destroyShaders();
    destroy();
}

ShaderProgram::ShaderProgram(const ShaderProgram& other) {
    *this = other;
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other) {

    if (this != &other) {

        create();
        for (const GLuint& shaderId : other.m_pPrivate->m_shaders)
            attachShader(shaderId);
    }

    return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
    *this = std::move(other);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

std::optional<GLuint> ShaderProgram::loadShader(const std::filesystem::path& path, GLenum shaderType) {

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
    m_pPrivate->m_shaders.emplace(shaderID);

    return shaderID;
}

bool ShaderProgram::attachShader(GLuint shaderID) const {

    if (glIsShader(shaderID) == GL_FALSE)
        return false;

    glAttachShader(m_pPrivate->m_program, shaderID);
    if (glGetError() == GL_INVALID_OPERATION) {
        std::cerr << "Error: Shader has already been attached to the given program\n";
        return false;
    }

    return true;
}

bool ShaderProgram::detachShader(GLuint shaderID) const {

    if (glIsShader(shaderID) == GL_FALSE)
        return false;

    glDetachShader(m_pPrivate->m_program, shaderID);
    if (glGetError() == GL_INVALID_OPERATION) {
        std::cerr << "Error: Shader has not been attached to the given program\n";
        return false;
    }

    return true;
}

bool ShaderProgram::destroyShader(GLuint shaderID) {

    if (glIsShader(shaderID) == GL_FALSE)
        return false;

    GLint isDeleted = GL_FALSE;
    glGetShaderiv(shaderID, GL_DELETE_STATUS, &isDeleted);

    if (isDeleted == GL_TRUE)
        return false;

    glDeleteShader(shaderID);

    return true;
}

bool ShaderProgram::destroyShaders() {

    // Private will be deleted if the program is moved.
    if (!m_pPrivate)
        return true;

    bool allDeleted = true;

    for (GLuint shader : m_pPrivate->m_shaders)
        allDeleted &= destroyShader(shader);

    return allDeleted;
}

void ShaderProgram::create() {
    m_pPrivate->m_program = glCreateProgram();
}

void ShaderProgram::destroy() {

    if (!m_pPrivate)
        return;

    if (glIsProgram(m_pPrivate->m_program) == GL_FALSE)
        return;

    GLint isDeleted = GL_FALSE;
    glGetProgramiv(m_pPrivate->m_program, GL_DELETE_STATUS, &isDeleted);

    if (isDeleted == GL_TRUE)
        return;

    glDeleteProgram(m_pPrivate->m_program);
}

bool ShaderProgram::compileAndLink() const {

    GLsizei numAttached = 0;

    std::vector<GLuint> attachedShaders;
    attachedShaders.resize(20);

    glGetAttachedShaders(m_pPrivate->m_program, static_cast<GLsizei>(attachedShaders.size()), &numAttached, attachedShaders.data());
    attachedShaders.resize(numAttached);

    // Bail early if there were no shaders attached
    if (!numAttached) {
        std::cerr << "Error: Cannot use a program that has no attached shaders\n";
        return false;
    }

    for (GLuint shader : attachedShaders) {
        glCompileShader(shader);
        if (!m_pPrivate->reportError(shader))
            return false;
    }

    glLinkProgram(m_pPrivate->m_program);
    if (!m_pPrivate->reportError(m_pPrivate->m_program))
        return false;

    m_pPrivate->readAttributes();
    m_pPrivate->readUniforms();

    return true;
}

void ShaderProgram::use() const {
    glUseProgram(m_pPrivate->m_program);
}

void ShaderProgram::set(const std::string& variable, GLboolean value) const {
    m_pPrivate->set(variable, value);
}

void ShaderProgram::set(const std::string& variable, GLint value) const {
    m_pPrivate->set(variable, value);
}

void ShaderProgram::set(const std::string& variable, GLfloat value) const {
    m_pPrivate->set(variable, value);
}

void ShaderProgram::set(const std::string& variable, glm::vec3 value) const {
    m_pPrivate->set(variable, value);
}

void ShaderProgram::set(const std::string& variable, glm::vec4 value) const {
    m_pPrivate->set(variable, value);
}

void ShaderProgram::set(const std::string& variable, glm::mat4 value) const {
    m_pPrivate->set(variable, value);
}

GLuint ShaderProgram::id() const {
    return m_pPrivate->m_program;
}

std::unordered_set<std::string_view> ShaderProgram::attributes() const {

    std::unordered_set<std::string_view> attributes;
    std::ranges::transform(m_pPrivate->m_attributes, std::inserter(attributes, attributes.cbegin()),
        [](const std::pair<std::string, GLuint>& pair) { return pair.first; });

    return attributes;
}

std::unordered_set<std::string_view> ShaderProgram::uniforms() const {

    std::unordered_set<std::string_view> uniforms;
    std::ranges::transform(m_pPrivate->m_uniforms, std::inserter(uniforms, uniforms.cbegin()),
        [](const std::string& uniform) { return uniform; });

    return uniforms;
}

std::optional<GLuint> ShaderProgram::attributeLocation(const std::string& attributeName) const {

    if (m_pPrivate->m_attributes.contains(attributeName))
        return m_pPrivate->m_attributes.at(attributeName);

    return std::nullopt;
}

bool ShaderProgram::hasAttribute(const std::string& name) const {
    return m_pPrivate->m_attributes.contains(name);
}

bool ShaderProgram::hasUniform(const std::string& name) const {
    return m_pPrivate->m_uniforms.contains(name);
}
