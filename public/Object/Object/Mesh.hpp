#pragma once

#include "Common/ClassMacros.hpp"
#include "Common/IRestorable.hpp"

#include <bitset>
#include <forward_list>
#include <optional>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <nlohmann/json.hpp>

class IMaterial;
class VertexBuffered;

class Mesh : public IRestorable {
public:
    Mesh();

    virtual std::string_view id() const override;
    virtual nlohmann::json save() const override;
    virtual void restore(const nlohmann::json& settings) override;

    void destroy() const;

    DECLARE_GETTER_IMMUTABLE_COPY(material, IMaterial*)
    DECLARE_SETTER_COPY(material, IMaterial*);

    DECLARE_GETTER_IMMUTABLE_COPY(model, std::forward_list<VertexBuffered>*)
    DECLARE_SETTER_CONSTREF(model, std::forward_list<VertexBuffered>)

    DECLARE_GETTER_IMMUTABLE_COPY(transform, glm::mat4)

    DECLARE_SETTER_COPY(scale, float)
    DECLARE_GETTER_IMMUTABLE_COPY(scale, float)

    DECLARE_SETTER_COPY(pitch, float)
    DECLARE_GETTER_IMMUTABLE_COPY(pitch, float)

    DECLARE_SETTER_COPY(yaw, float)
    DECLARE_GETTER_IMMUTABLE_COPY(yaw, float)

    DECLARE_SETTER_COPY(roll, float)
    DECLARE_GETTER_IMMUTABLE_COPY(roll, float)

    DECLARE_SETTER_CONSTREF(translate, glm::vec3)
    DECLARE_GETTER_IMMUTABLE_COPY(translate, glm::vec3)

    DECLARE_SETTER_CONSTREF(position, glm::vec3)
    DECLARE_GETTER_IMMUTABLE_COPY(position, glm::vec3)

    DECLARE_GETTER_IMMUTABLE_COPY(faceCount, std::uint32_t)
    DECLARE_GETTER_IMMUTABLE_COPY(vertexCount, std::uint32_t)

    DECLARE_GETTER_IMMUTABLE_COPY(hasColors, bool)
    DECLARE_GETTER_IMMUTABLE_COPY(hasIndices, bool)
    DECLARE_GETTER_IMMUTABLE_COPY(hasNormals, bool)
    DECLARE_GETTER_IMMUTABLE_COPY(hasPositions, bool)
    DECLARE_GETTER_IMMUTABLE_COPY(hasTexels, bool)

    DECLARE_GETTER_IMMUTABLE_COPY(initialized, bool);
    DECLARE_SETTER_COPY(initialized, bool);

private:
    COMPILATION_FIREWALL_COPY_MOVE(Mesh);
};
