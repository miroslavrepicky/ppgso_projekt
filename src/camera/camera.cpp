#include "camera.h"
#include <glm/gtx/quaternion.hpp>

namespace ppgso {

    Camera::Camera()
        : fieldOfView(glm::radians(45.0f))
        , aspectRatio(16.0f / 9.0f)
        , nearPlane(0.1f)
        , farPlane(1000.0f)
        , target(0.0f, 0.0f, 0.0f)
        , up(0.0f, 1.0f, 0.0f)
    {
        transform.setPosition(glm::vec3(0.0f, 5.0f, 10.0f));
    }

    Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
        : fieldOfView(fov)
        , aspectRatio(aspectRatio)
        , nearPlane(nearPlane)
        , farPlane(farPlane)
        , target(0.0f, 0.0f, 0.0f)
        , up(0.0f, 1.0f, 0.0f)
    {
        transform.setPosition(glm::vec3(0.0f, 5.0f, 10.0f));
    }

    // View matica
    glm::mat4 Camera::getViewMatrix() const {
        glm::vec3 position = transform.getPosition();
        
        // Ak pouzivame quaterniony, mozeme ich pouzit priamo
        // Inak pouzijeme lookAt
        return glm::lookAt(position, target, up);
    }

    // Projection matica
    glm::mat4 Camera::getProjectionMatrix() const {
        return glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    }

    // ViewProjection matica
    glm::mat4 Camera::getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }

    // Pozicia
    void Camera::setPosition(const glm::vec3& position) {
        transform.setPosition(position);
    }

    glm::vec3 Camera::getPosition() const {
        return transform.getPosition();
    }

    // LookAt
    void Camera::lookAt(const glm::vec3& target) {
        this->target = target;
        
        // Vypocitaj rotaciu na zaklade smeru
        glm::vec3 position = transform.getPosition();
        glm::vec3 direction = glm::normalize(target - position);
        
        // Vypocitaj quaternion z direction vektora
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::quat rotation = glm::rotation(forward, direction);
        transform.setRotation(rotation);
    }

    void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
        this->target = target;
        this->up = up;
        lookAt(target);
    }

    // Perspektiva
    void Camera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
        this->fieldOfView = fov;
        this->aspectRatio = aspectRatio;
        this->nearPlane = nearPlane;
        this->farPlane = farPlane;
    }

    void Camera::setFieldOfView(float fov) {
        this->fieldOfView = fov;
    }

    void Camera::setAspectRatio(float aspectRatio) {
        this->aspectRatio = aspectRatio;
    }

    void Camera::setNearPlane(float nearPlane) {
        this->nearPlane = nearPlane;
    }

    void Camera::setFarPlane(float farPlane) {
        this->farPlane = farPlane;
    }

    // Gettery
    float Camera::getFieldOfView() const {
        return fieldOfView;
    }

    float Camera::getAspectRatio() const {
        return aspectRatio;
    }

    float Camera::getNearPlane() const {
        return nearPlane;
    }

    float Camera::getFarPlane() const {
        return farPlane;
    }

    // Transform
    Transform& Camera::getTransform() {
        return transform;
    }

    const Transform& Camera::getTransform() const {
        return transform;
    }

    // Smery kamery
    glm::vec3 Camera::getForward() const {
        return glm::normalize(target - transform.getPosition());
    }

    glm::vec3 Camera::getRight() const {
        return glm::normalize(glm::cross(getForward(), up));
    }

    glm::vec3 Camera::getUp() const {
        return glm::normalize(glm::cross(getRight(), getForward()));
    }

    // Update
    void Camera::update(float deltaTime) {
        // Zakladna implementacia - prazdna
        // Neskor tu bude keyframe animacia
    }

} // namespace ppgso