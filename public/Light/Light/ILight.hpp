#pragma once

class ShaderProgram;

class ILight {
public:
    virtual void apply(ShaderProgram* pShader) const = 0;
};
