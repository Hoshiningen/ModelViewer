#pragma once

#include "UI/Components/IComponent.hpp"

#include <sigslot/signal.hpp>

class TitleBarComponent : public IComponent {
public:

private:
    virtual void render() override;
};
