#ifndef PPGSO_POINT_LIGHT_H
#define PPGSO_POINT_LIGHT_H

#include "light.h"

namespace ppgso {

    /**
     * PointLight - Bodove svetlo (napr. lampicka, fakla)
     * Ma poziciu a tlmi sa so vzdialenostou
     */
    class PointLight : public Light {
    public:
        PointLight();
        PointLight(const glm::vec3& position);

        // Pozicia svetla
        void setPosition(const glm::vec3& position);
        glm::vec3 getPosition() const;

        // Attenuation (tlmenie so vzdialenostou)
        // Intensity = 1.0 / (constant + linear*d + quadratic*d^2)
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;

        // Pomocna metoda pre nastavenie attenuation podla dosahu
        void setRange(float range);

        // Override
        void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) override;

    private:
        glm::vec3 position;
    };

} // namespace ppgso

#endif // PPGSO_POINT_LIGHT_H