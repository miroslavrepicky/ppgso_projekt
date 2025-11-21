#ifndef PPGSO_DIRECTIONAL_LIGHT_H
#define PPGSO_DIRECTIONAL_LIGHT_H

#include "light.h"

namespace ppgso {

    /**
     * DirectionalLight - Smerove svetlo (napr. slnko)
     * Ma smer, ale nie poziciu (svetlo prichádza z nekonecna)
     */
    class DirectionalLight : public Light {
    public:
        DirectionalLight();
        DirectionalLight(const glm::vec3& direction);

        // Smer svetla (normalizovany vektor)
        void setDirection(const glm::vec3& direction);
        glm::vec3 getDirection() const;

        // Override
        void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) override;

    private:
        glm::vec3 direction;
    };

} // namespace ppgso

#endif // PPGSO_DIRECTIONAL_LIGHT_H