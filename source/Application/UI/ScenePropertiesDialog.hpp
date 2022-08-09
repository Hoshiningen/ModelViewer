#include "UI/Dialog.hpp"

#include "Common/SignalMacros.hpp"

#include "Object/Mesh.hpp"

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <sigslot/signal.hpp>

class ScenePropertiesDialog : public Dialog {
public:
    using Dialog::Dialog;

    enum Projection {
        ePerspective, eOrthographic
    };

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    glm::vec3* clearColor();
    glm::vec3* ambientColor();
    float* ambientIntensity();

    void onProjectionChange(int projection);
    void onWireframeModeChange(bool wireframeEnabled);

    void mesh(Mesh* pMesh);

protected:
    virtual void defineUI() override;

private:
    glm::vec3 m_clearColor{ 0.305, 0.520, 0.828 };
    glm::vec3 m_ambientColor{ 1.f };
    float m_ambientIntensity = 0.f;

    int m_projection = Projection::ePerspective;
    bool m_wireframe = false;

    Mesh* m_pMesh = nullptr;
};
