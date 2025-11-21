#include "light.h"

namespace ppgso {

    Light::Light(LightType type)
        : type(type)
        , ambient(0.1f, 0.1f, 0.1f)
        , diffuse(1.0f, 1.0f, 1.0f)
        , specular(1.0f, 1.0f, 1.0f)
        , intensity(1.0f)
        , enabled(true)
    {
    }

    Light::~Light() {
    }

    LightType Light::getType() const {
        return type;
    }

    void Light::setColor(const glm::vec3& color) {
        // Nastav vsetky zlozky na rovnaku farbu
        ambient = color * 0.2f;  // Ambient je tmavsi
        diffuse = color;
        specular = color;
    }

    void Light::setAmbient(const glm::vec3& ambient) {
        this->ambient = ambient;
    }

    void Light::setDiffuse(const glm::vec3& diffuse) {
        this->diffuse = diffuse;
    }

    void Light::setSpecular(const glm::vec3& specular) {
        this->specular = specular;
    }

    std::string Light::getUniformName(int lightIndex, const std::string& property) const {
        // Napr: "lights[0].ambient"
        return "lights[" + std::to_string(lightIndex) + "]." + property;
    }

} // namespace ppgso