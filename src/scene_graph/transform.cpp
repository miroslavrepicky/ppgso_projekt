#include "transform.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace ppgso {

    Transform::Transform()
        : position(0.0f, 0.0f, 0.0f)
        , rotation(1.0f, 0.0f, 0.0f, 0.0f) // identity quaternion
        , scale(1.0f, 1.0f, 1.0f)
        , parent(nullptr)
        , localMatrixDirty(true)
    {
    }

    Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
        : position(position)
        , rotation(rotation)
        , scale(scale)
        , parent(nullptr)
        , localMatrixDirty(true)
    {
    }

    // Pozicia
    void Transform::setPosition(const glm::vec3& position) {
        this->position = position;
        localMatrixDirty = true;
    }

    glm::vec3 Transform::getPosition() const {
        return position;
    }

    void Transform::translate(const glm::vec3& offset) {
        position += offset;
        localMatrixDirty = true;
    }

    // Rotacia
    void Transform::setRotation(const glm::quat& rotation) {
        this->rotation = rotation;
        localMatrixDirty = true;
    }

    void Transform::setRotation(const glm::vec3& eulerAngles) {
        rotation = glm::quat(eulerAngles);
        localMatrixDirty = true;
    }

    glm::quat Transform::getRotation() const {
        return rotation;
    }

    void Transform::rotate(const glm::quat& rotation) {
        this->rotation = rotation * this->rotation;
        localMatrixDirty = true;
    }

    void Transform::rotate(float angle, const glm::vec3& axis) {
        rotation = glm::angleAxis(angle, glm::normalize(axis)) * rotation;
        localMatrixDirty = true;
    }

    // Skala
    void Transform::setScale(const glm::vec3& scale) {
        this->scale = scale;
        localMatrixDirty = true;
    }

    void Transform::setScale(float uniformScale) {
        this->scale = glm::vec3(uniformScale);
        localMatrixDirty = true;
    }

    glm::vec3 Transform::getScale() const {
        return scale;
    }

    // Maticove operacie
    glm::mat4 Transform::getLocalMatrix() const {
        if (localMatrixDirty) {
            // TRS (Translate, Rotate, Scale) transformacia
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 rotationMatrix = glm::toMat4(rotation);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
            
            cachedLocalMatrix = translationMatrix * rotationMatrix * scaleMatrix;
            localMatrixDirty = false;
        }
        return cachedLocalMatrix;
    }

    glm::mat4 Transform::getWorldMatrix() const {
        if (parent != nullptr) {
            // Kombinacia s parent transformaciou
            return parent->getWorldMatrix() * getLocalMatrix();
        }
        return getLocalMatrix();
    }

    // Hierarchia
    void Transform::setParent(Transform* parent) {
        this->parent = parent;
    }

    Transform* Transform::getParent() const {
        return parent;
    }

    // Pomocne smery v lokalnom priestore
    glm::vec3 Transform::getForward() const {
        return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    glm::vec3 Transform::getRight() const {
        return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 Transform::getUp() const {
        return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // Pomocne smery v svetovom priestore
    glm::vec3 Transform::getWorldForward() const {
        glm::mat4 worldMat = getWorldMatrix();
        return glm::normalize(glm::vec3(worldMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
    }

    glm::vec3 Transform::getWorldRight() const {
        glm::mat4 worldMat = getWorldMatrix();
        return glm::normalize(glm::vec3(worldMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
    }

    glm::vec3 Transform::getWorldUp() const {
        glm::mat4 worldMat = getWorldMatrix();
        return glm::normalize(glm::vec3(worldMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
    }

} // namespace ppgso