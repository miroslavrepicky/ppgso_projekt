#include "spot_light.h"

namespace ppgso {

    SpotLight::SpotLight()
        : Light(LightType::SPOT)
        , position(0.0f, 5.0f, 0.0f)
        , direction(0.0f, -1.0f, 0.0f)
        , innerCutoff(glm::cos(glm::radians(12.5f)))
        , outerCutoff(glm::cos(glm::radians(17.5f)))
        , constantAttenuation(1.0f)
        , linearAttenuation(0.09f)
        , quadraticAttenuation(0.032f)
    {
        setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction)
        : Light(LightType::SPOT)
        , position(position)
        , direction(glm::normalize(direction))
        , innerCutoff(glm::cos(glm::radians(12.5f)))
        , outerCutoff(glm::cos(glm::radians(17.5f)))
        , constantAttenuation(1.0f)
        , linearAttenuation(0.09f)
        , quadraticAttenuation(0.032f)
    {
        setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    void SpotLight::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    glm::vec3 SpotLight::getPosition() const {
        return position;
    }

    void SpotLight::setDirection(const glm::vec3& direction) {
        this->direction = glm::normalize(direction);
    }

    glm::vec3 SpotLight::getDirection() const {
        return direction;
    }

    void SpotLight::setCutoff(float innerDegrees, float outerDegrees) {
        innerCutoff = glm::cos(glm::radians(innerDegrees));
        outerCutoff = glm::cos(glm::radians(outerDegrees));
    }

    void SpotLight::setRange(float range) {
        constantAttenuation = 1.0f;
        linearAttenuation = 4.5f / range;
        quadraticAttenuation = 75.0f / (range * range);
    }

    void SpotLight::setupShaderUniforms(ppgso::Shader& shader, int lightIndex) {
        if (!enabled) {
            shader.setUniform(getUniformName(lightIndex, "enabled"), 0);
            return;
        }

        shader.setUniform(getUniformName(lightIndex, "type"), 2);
        shader.setUniform(getUniformName(lightIndex, "position"), position);
        shader.setUniform(getUniformName(lightIndex, "direction"), direction);
        shader.setUniform(getUniformName(lightIndex, "ambient"), ambient * intensity);
        shader.setUniform(getUniformName(lightIndex, "diffuse"), diffuse * intensity);
        shader.setUniform(getUniformName(lightIndex, "specular"), specular * intensity);
        shader.setUniform(getUniformName(lightIndex, "innerCutoff"), innerCutoff);
        shader.setUniform(getUniformName(lightIndex, "outerCutoff"), outerCutoff);
        shader.setUniform(getUniformName(lightIndex, "constantAttenuation"), constantAttenuation);
        shader.setUniform(getUniformName(lightIndex, "linearAttenuation"), linearAttenuation);
        shader.setUniform(getUniformName(lightIndex, "quadraticAttenuation"), quadraticAttenuation);
        shader.setUniform(getUniformName(lightIndex, "enabled"), 1);
    }

} // namespace ppgso