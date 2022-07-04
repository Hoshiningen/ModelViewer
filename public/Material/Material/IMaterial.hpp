#pragma once

class ShaderProgram;

class IMaterial {
public:
    virtual ~IMaterial() = default;
    virtual void apply(ShaderProgram* pShader) const = 0;
    virtual void wireframe(bool value) = 0;
};
