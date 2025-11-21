#ifndef PPGSO_TRANSFORM_H
#define PPGSO_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ppgso {

    /**
     * Transform - Reprezentuje poziciu, rotaciu a skalu objektu
     * Podporuje hierarchicke transformacie pomocou parent-child vztahov
     */
    class Transform {
    public:
        // Konstruktory
        Transform();
        Transform(const glm::vec3& position, const glm::quat& rotation = glm::quat(1,0,0,0), const glm::vec3& scale = glm::vec3(1.0f));

        // Pozicia
        void setPosition(const glm::vec3& position);
        glm::vec3 getPosition() const;
        void translate(const glm::vec3& offset);

        // Rotacia (pomocou quaternionov pre stabilne interpolacie)
        void setRotation(const glm::quat& rotation);
        void setRotation(const glm::vec3& eulerAngles); // pomocna metoda pre Euler angles
        glm::quat getRotation() const;
        void rotate(const glm::quat& rotation);
        void rotate(float angle, const glm::vec3& axis);

        // Skala
        void setScale(const glm::vec3& scale);
        void setScale(float uniformScale);
        glm::vec3 getScale() const;

        // Maticove operacie
        glm::mat4 getLocalMatrix() const;      // Lokalna transformacna matica
        glm::mat4 getWorldMatrix() const;      // Svetova transformacna matica (s parentom)

        // Hierarchia
        void setParent(Transform* parent);
        Transform* getParent() const;

        // Pomocne smery v lokalnom priestore
        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;

        // Pomocne smery v svetovom priestore
        glm::vec3 getWorldForward() const;
        glm::vec3 getWorldRight() const;
        glm::vec3 getWorldUp() const;

    private:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        Transform* parent;

        // Cache pre optimalizaciu (volitelne)
        mutable glm::mat4 cachedLocalMatrix;
        mutable bool localMatrixDirty;
    };

} // namespace ppgso

#endif // PPGSO_TRANSFORM_H