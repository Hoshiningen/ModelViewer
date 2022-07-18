#pragma once

#include "Common/IRestorable.hpp"

class ShaderProgram;

class ILight : public IRestorable {
public:
    virtual void apply(ShaderProgram* pShader) const = 0;
};
