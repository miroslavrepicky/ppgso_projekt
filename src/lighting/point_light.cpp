#include "point_light.h"
#include <cmath>

namespace ppgso {

    PointLight::PointLight()
        : Light(LightType::POINT)
        , position(0.0f, 5.0f, 0.0f)
        , constantAttenuation(1.0f)
        , linearAttenuation(0.09f)
        , quadraticAttenuation(0.032f)
    {
        setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        intensity = 1.0f;
    }

    PointLight::PointLight(const glm::vec3& position)
        : Light(LightType::POINT)
        , position(position)
        , constantAttenuation(1.0f)
        , linearAttenuation(0.09f)
        , quadraticAttenuation(0.032f)
    {
        setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    void PointLight::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    glm::vec3 PointLight::getPosition() const {
        return position;
    }

    void PointLight::setRange(float range) {
        // Vypocitaj attenuation faktory pre dany dosah
        // Na okraji dosahu bude intenzita ~5% povodnej
        constantAttenuation = 1.0f;
        linearAttenuation = 4.5f / range;
        quadraticAttenuation = 75.0f / (range * range);
    }

    void PointLight::setupShaderUniforms(ppgso::Shader& shader, int lightIndex) {
        if (!enabled) {
            shader.setUniform(getUniformName(lightIndex, "enabled"), 0);
            return;
        }

        // Type
        shader.setUniform(getUniformName(lightIndex, "type"), 1); // POINT

        // Position
        shader.setUniform(getUniformName(lightIndex, "position"), position);

        // Colors
        shader.setUniform(getUniformName(lightIndex, "ambient"), ambient * intensity);
        shader.setUniform(getUniformName(lightIndex, "diffuse"), diffuse * intensity);
        shader.setUniform(getUniformName(lightIndex, "specular"), specular * intensity);

        // Attenuation
        shader.setUniform(getUniformName(lightIndex, "constantAttenuation"), constantAttenuation);
        shader.setUniform(getUniformName(lightIndex, "linearAttenuation"), linearAttenuation);
        shader.setUniform(getUniformName(lightIndex, "quadraticAttenuation"), quadraticAttenuation);

        // Enabled
        shader.setUniform(getUniformName(lightIndex, "enabled"), 1);
    }

} // namespace ppgso