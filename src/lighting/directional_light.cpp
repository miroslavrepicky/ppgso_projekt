#include "directional_light.h"

namespace ppgso {

    DirectionalLight::DirectionalLight()
        : Light(LightType::DIRECTIONAL)
        , direction(0.0f, -1.0f, 0.0f)
    {
        setColor(glm::vec3(1.0f, 0.95f, 0.8f));
        diffuse = glm::vec3(1.2f, 1.15f, 1.0f);
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
        if (!enabled) {
            shader.setUniform(getUniformName(lightIndex, "enabled"), 0);
            return;
        }

        shader.setUniform(getUniformName(lightIndex, "type"), 0);
        shader.setUniform(getUniformName(lightIndex, "direction"), direction);
        shader.setUniform(getUniformName(lightIndex, "ambient"), ambient * intensity);
        shader.setUniform(getUniformName(lightIndex, "diffuse"), diffuse * intensity);
        shader.setUniform(getUniformName(lightIndex, "specular"), specular * intensity);
        shader.setUniform(getUniformName(lightIndex, "enabled"), 1);
    }

} // namespace ppgso