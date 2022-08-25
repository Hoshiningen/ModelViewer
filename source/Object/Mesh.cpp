#include "Object/Mesh.hpp"

#include "Common/Math.hpp"
#include "Geometry/VertexBuffered.hpp"
#include "Material/IMaterial.hpp"

namespace {
struct Metadata {
    std::uint32_t vertexCount = 0;
    std::uint32_t faceCount = 0;
    std::bitset<5> attributes;

    void setAttribute(VertexBuffered::NamedAttribute attribute, bool isSet);
    bool hasAttribute(VertexBuffered::NamedAttribute attribute) const;
};

void Metadata::setAttribute(VertexBuffered::NamedAttribute attribute, bool isSet) {
    attributes.set(static_cast<std::size_t>(attribute), isSet);
}

bool Metadata::hasAttribute(VertexBuffered::NamedAttribute attribute) const {
    return attributes.test(static_cast<std::size_t>(attribute));
}

Metadata readMetadata(const std::forward_list<VertexBuffered>& model) {

    using enum VertexBuffered::NamedAttribute;

    Metadata metadata;
    if (model.empty())
        return metadata;

    std::size_t indexCount = 0;
    for (const VertexBuffered& buffer : model) {

        const auto vertices = buffer.vertices();
        const auto indices = buffer.indices();

        metadata.vertexCount += vertices->size();
        indexCount += indices->size();

        metadata.setAttribute(Color, buffer.colors().has_value());
        metadata.setAttribute(Normal, buffer.normals().has_value());
        metadata.setAttribute(Texel, buffer.texels().has_value());
    }

    metadata.setAttribute(Position, metadata.vertexCount > 0);
    metadata.setAttribute(Index, indexCount > 0);
    metadata.faceCount = indexCount > 0 ? indexCount / 3 : 0;

    return metadata;
}
} // end unnamed namespace

struct Mesh::Private {
    std::forward_list<VertexBuffered> m_model;
    IMaterial* m_pMaterial = nullptr;
    glm::vec3 m_position{ 0.f };

    float m_scale = 1.f;
    float m_pitch= 0.f;
    float m_yaw = 0.f;
    float m_roll = 0.f;
    glm::vec3 m_translate{ 0.f };

    Metadata m_metadata;

    bool m_initialized = false;
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

void Mesh::material(IMaterial* pMaterial) {
    m_pPrivate->m_pMaterial = pMaterial;
    m_pPrivate->m_initialized = false;
}

DEFINE_GETTER_IMMUTABLE_COPY(Mesh, model, std::forward_list<VertexBuffered>*, &m_pPrivate->m_model)

void Mesh::model(const std::forward_list<VertexBuffered>& model) {
    m_pPrivate->m_model = model;
    m_pPrivate->m_metadata = readMetadata(m_pPrivate->m_model);
    m_pPrivate->m_initialized = false;
}

glm::mat4 Mesh::transform() const {

    // Create a transformation matrix composed of matrices in the order of scale, rotate, then translate.
    const glm::mat4 scaleMat = glm::scale(glm::identity<glm::mat4>(), glm::vec3{m_pPrivate->m_scale});
    const glm::mat4 rotMat = glm::mat4_cast(MakeQuat(m_pPrivate->m_pitch, m_pPrivate->m_yaw, m_pPrivate->m_roll, true));
    const glm::mat4 transMat = glm::translate(glm::identity<glm::mat4>(), m_pPrivate->m_translate + m_pPrivate->m_position);

    return rotMat * scaleMat * transMat;
}

DEFINE_SETTER_COPY(Mesh, scale, m_pPrivate->m_scale)
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, scale, float, m_pPrivate->m_scale)

DEFINE_SETTER_COPY(Mesh, pitch, m_pPrivate->m_pitch)
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, pitch, float, m_pPrivate->m_pitch)

DEFINE_SETTER_COPY(Mesh, yaw, m_pPrivate->m_yaw)
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, yaw, float, m_pPrivate->m_yaw)

DEFINE_SETTER_COPY(Mesh, roll, m_pPrivate->m_roll)
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, roll, float, m_pPrivate->m_roll)

DEFINE_SETTER_CONSTREF(Mesh, translate, m_pPrivate->m_translate)
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, translate, glm::vec3, m_pPrivate->m_translate)

DEFINE_SETTER_CONSTREF(Mesh, position, m_pPrivate->m_position)
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, position, glm::vec3, m_pPrivate->m_position)

DEFINE_GETTER_IMMUTABLE_COPY(Mesh, faceCount, std::uint32_t, m_pPrivate->m_metadata.faceCount)
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, vertexCount, std::uint32_t, m_pPrivate->m_metadata.vertexCount)

DEFINE_GETTER_IMMUTABLE_COPY(Mesh, hasColors, bool, m_pPrivate->m_metadata.hasAttribute(VertexBuffered::NamedAttribute::Color))
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, hasIndices, bool, m_pPrivate->m_metadata.hasAttribute(VertexBuffered::NamedAttribute::Index))
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, hasNormals, bool, m_pPrivate->m_metadata.hasAttribute(VertexBuffered::NamedAttribute::Normal))
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, hasPositions, bool, m_pPrivate->m_metadata.hasAttribute(VertexBuffered::NamedAttribute::Position))
DEFINE_GETTER_IMMUTABLE_COPY(Mesh, hasTexels, bool, m_pPrivate->m_metadata.hasAttribute(VertexBuffered::NamedAttribute::Texel))

DEFINE_GETTER_IMMUTABLE_COPY(Mesh, initialized, bool, m_pPrivate->m_initialized);
DEFINE_SETTER_COPY(Mesh, initialized, m_pPrivate->m_initialized);
