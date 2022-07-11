#include "UI/Dialog.hpp"

#include <glm/vec3.hpp>
#include <sigslot/signal.hpp>

class ScenePropertiesDialog : public Dialog {
public:
    using Dialog::Dialog;

    enum Projection {
        ePerspective, eOrthographic
    };

    template<typename... CallArgs>
    sigslot::connection connectClearColorChanged(CallArgs&&... args) {
        return m_clearColorChanged.connect(std::forward<CallArgs>(args)...);
    }

    void onProjectionChange(int projection);
    void onWireframeModeChange(bool wireframeEnabled);

protected:
    virtual void defineUI() override;

private:
    sigslot::signal<glm::vec3> m_clearColorChanged;

    glm::vec3 m_clearColor;
    int m_projection = Projection::ePerspective;
    bool m_wireframe = false;
};
