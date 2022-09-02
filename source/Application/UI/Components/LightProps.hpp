#pragma once

#include "Common/ClassMacros.hpp"
#include "UI/Components/IComponent.hpp"
#include "UI/Components/DirectionalLightProps.hpp"

#include <array>
#include <bitset>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sigslot/signal.hpp>

class LightProps : public IComponent {
public:
    virtual void render() override;

    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;

    virtual void compose(const std::initializer_list<IComponent*>& components) override;

    struct DataModel : public IComponent::DataModel {
        std::array<bool, 3> m_enabledLights;
    };

private:
    DataModel m_model;

    std::array<DirectionalLightProps*, 3> m_lightProps;
};
