#include "directional_light.h"

namespace ppgso {

    DirectionalLight::DirectionalLight()
        : Light(LightType::DIRECTIONAL)
        , direction(0.0f, -1.0f, 0.0f) // Svieti dole
    {
        // Slnko - zlta farba, silne difuzne svetlo
        setColor(glm::vec3(1.0f, 0.95f, 0.8f));
        diffuse = glm::vec3(1.2f, 1.15f, 1.0f); // HDR - viac ako 1.0
        intensity = 1.0f;
    }

    DirectionalLight::DirectionalLight(const glm::vec3& direction)
        : Light(LightType::DIRECTIONAL)
        , direction(glm::normalize(direction))
    {
        setColor(glm::vec3(1.0f, 0.95f, 0.8f));
    }

    void DirectionalLight::setDirection(const glm::vec3& direction) {
        this->direction = glm::normalize(direction);
    }

    glm::vec3 DirectionalLight::getDirection() const {
        return direction;
    }

    void DirectionalLight::setupShaderUniforms(ppgso::Shader& shader, int lightIndex) {
        if (!enabled) return;

        // Typ svetla
        shader.setUniform(getUniformName(lightIndex, "type"), 0); // 0 = DIRECTIONAL

        // Smer
        shader.setUniform(getUniformName(lightIndex, "direction"), direction);

        // Farby
        shader.setUniform(getUniformName(lightIndex, "ambient"), ambient * intensity);
        shader.setUniform(getUniformName(lightIndex, "diffuse"), diffuse * intensity);
        shader.setUniform(getUniformName(lightIndex, "specular"), specular * intensity);

        // Enabled
        shader.setUniform(getUniformName(lightIndex, "enabled"), enabled ? 1 : 0);
    }

} // namespace ppgso