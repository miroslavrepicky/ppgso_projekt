#ifndef PPGSO_CAMERA_H
#define PPGSO_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../scene_graph/transform.h"

namespace ppgso {

    /**
     * Camera - Reprezentuje kameru v 3D priestore
     * Podporuje perspektivnu projekciu a pohyb v priestore
     */
    class Camera {
    public:
        // Konstruktory
        Camera();
        Camera(float fov, float aspectRatio, float nearPlane, float farPlane);

        // View matica (pozicia a smer kamery)
        glm::mat4 getViewMatrix() const;

        // Projection matica (perspektivna projekcia)
        glm::mat4 getProjectionMatrix() const;

        // Kombinovana ViewProjection matica
        glm::mat4 getViewProjectionMatrix() const;

        // Pozicia a orientacia kamery
        void setPosition(const glm::vec3& position);
        glm::vec3 getPosition() const;

        void lookAt(const glm::vec3& target);
        void lookAt(const glm::vec3& target, const glm::vec3& up);

        // Nastavenie perspektivy
        void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
        void setFieldOfView(float fov);
        void setAspectRatio(float aspectRatio);
        void setNearPlane(float nearPlane);
        void setFarPlane(float farPlane);

        // Gettery pre parametre
        float getFieldOfView() const;
        float getAspectRatio() const;
        float getNearPlane() const;
        float getFarPlane() const;

        // Transform (pre hierarchicke transformacie)
        Transform& getTransform();
        const Transform& getTransform() const;

        // Smery kamery
        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;

        // Update (pre buduce animacie)
        virtual void update(float deltaTime);

    protected:
        Transform transform;

        // Projection parametre
        float fieldOfView;    // v radianoch
        float aspectRatio;
        float nearPlane;
        float farPlane;

        // Target (kam sa pozerá kamera)
        glm::vec3 target;
        glm::vec3 up;
    };

} // namespace ppgso

#endif // PPGSO_CAMERA_H