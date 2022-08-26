#pragma once

#include "Common/IRestorable.hpp"

class ShaderProgram;

class IMaterial : public IRestorable {
public:
    virtual ~IMaterial() = default;
    virtual void apply(ShaderProgram* pShader) const = 0;

    virtual void destroy() {}
};
