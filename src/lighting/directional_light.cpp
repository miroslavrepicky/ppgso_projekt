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
        std::string prefix = "light" + std::to_string(lightIndex) + "_";

        if (!enabled) {
            shader.setUniform(prefix + "enabled", 0);
            return;
        }

        // Debug output
        static int debugCount = 0;
        if (debugCount++ % 120 == 0) {
            std::cout << "DirectionalLight[" << lightIndex << "] setting uniforms:" << std::endl;
            std::cout << "  direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
            std::cout << "  uniform name: " << prefix + "direction" << std::endl;
        }

        // Type
        shader.setUniform(prefix + "type", 0); // DIRECTIONAL

        // Direction
        shader.setUniform(prefix + "direction", direction);

        // Colors
        shader.setUniform(prefix + "ambient", ambient * intensity);
        shader.setUniform(prefix + "diffuse", diffuse * intensity);
        shader.setUniform(prefix + "specular", specular * intensity);

        // Enabled
        shader.setUniform(prefix + "enabled", 1);
    }

} // namespace ppgso