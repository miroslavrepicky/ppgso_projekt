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
        ambient = color * 0.2f;
        diffuse = color;
        specular = color;
    }

    std::string Light::getUniformName(int lightIndex, const std::string& property) const {
        return "light" + std::to_string(lightIndex) + "_" + property;
    }

} // namespace ppgso