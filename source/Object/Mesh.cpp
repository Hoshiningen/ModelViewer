#include "Object/Mesh.hpp"

#include "Common/Math.hpp"
#include "Geometry/VertexBuffered.hpp"
#include "Material/IMaterial.hpp"

struct Mesh::Private {
    std::forward_list<VertexBuffered> m_model;
    IMaterial* m_pMaterial = nullptr;
    glm::vec3 m_position{ 0.f };

    float m_scale = 1.f;
    float m_pitch= 0.f;
    float m_yaw = 0.f;
    float m_roll = 0.f;
    glm::vec3 m_translate{ 0.f };
};


Mesh::Mesh()
    : m_pPrivate(std::make_unique<Private>()) {}

Mesh::~Mesh() noexcept {}

Mesh::Mesh(const Mesh& other) {
    *this = other;
}

Mesh& Mesh::operator=(const Mesh& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

Mesh::Mesh(Mesh&& other) noexcept {
    *this = std::move(other);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

std::string_view Mesh::id() const {
    return "Mesh";
}

nlohmann::json Mesh::save() const {

    nlohmann::json json;
    json[id().data()]["scale"] = m_pPrivate->m_scale;
    json[id().data()]["pitch"] = m_pPrivate->m_pitch;
    json[id().data()]["yaw"] = m_pPrivate->m_yaw;
    json[id().data()]["roll"] = m_pPrivate->m_roll;
    json[id().data()]["translate"] = m_pPrivate->m_translate;
    json[id().data()]["origin"] = m_pPrivate->m_position;

    return json;
}

void Mesh::restore(const nlohmann::json& settings) {

    if (!settings.is_object())
        return;

    if (settings.contains("scale"))
        settings["scale"].get_to(m_pPrivate->m_scale);

    if (settings.contains("pitch"))
        settings["pitch"].get_to(m_pPrivate->m_pitch);

    if (settings.contains("yaw"))
        settings["yaw"].get_to(m_pPrivate->m_yaw);

    if (settings.contains("roll"))
        settings["roll"].get_to(m_pPrivate->m_roll);

    if (settings.contains("translate"))
        settings["translate"].get_to(m_pPrivate->m_translate);

    if (settings.contains("origin"))
        settings["origin"].get_to(m_pPrivate->m_position);
}

DEFINE_GETTER_IMMUTABLE_COPY(Mesh, material, IMaterial*, m_pPrivate->m_pMaterial)
DEFINE_SETTER_COPY(Mesh, material, m_pPrivate->m_pMaterial)

DEFINE_GETTER_IMMUTABLE_COPY(Mesh, model, std::forward_list<VertexBuffered>*, &m_pPrivate->m_model)
DEFINE_SETTER_CONSTREF(Mesh, model, m_pPrivate->m_model)

glm::mat4 Mesh::transform() const {

    // Create a transformation matrix composed of matrices in the order of scale, rotate, then translate.
    const glm::mat4 scaleMat = glm::scale(glm::identity<glm::mat4>(), glm::vec3{m_pPrivate->m_scale});
    const glm::mat4 rotMat = glm::mat4_cast(MakeQuat(m_pPrivate->m_pitch, m_pPrivate->m_yaw, m_pPrivate->m_roll, true));
    const glm::mat4 transMat = glm::translate(glm::identity<glm::mat4>(), m_pPrivate->m_translate + m_pPrivate->m_position);

    return rotMat * scaleMat * transMat;
}

DEFINE_GETTER_MUTABLE(Mesh, scale, float, m_pPrivate->m_scale)
DEFINE_GETTER_MUTABLE(Mesh, pitch, float, m_pPrivate->m_pitch)
DEFINE_GETTER_MUTABLE(Mesh, yaw, float, m_pPrivate->m_yaw)
DEFINE_GETTER_MUTABLE(Mesh, roll, float, m_pPrivate->m_roll)
DEFINE_GETTER_MUTABLE(Mesh, translate, glm::vec3, m_pPrivate->m_translate)
DEFINE_GETTER_MUTABLE(Mesh, position, glm::vec3, m_pPrivate->m_position)
