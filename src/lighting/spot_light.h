#ifndef PPGSO_SPOT_LIGHT_H
#define PPGSO_SPOT_LIGHT_H

#include "light.h"

namespace ppgso {

    /**
     * SpotLight - Reflektor (napr. baterka, spotlight)
     * Ma poziciu, smer a kuzelovy uhol
     */
    class SpotLight : public Light {
    public:
        SpotLight();
        SpotLight(const glm::vec3& position, const glm::vec3& direction);

        // Pozicia svetla
        void setPosition(const glm::vec3& position);
        glm::vec3 getPosition() const;

        // Smer svetla
        void setDirection(const glm::vec3& direction);
        glm::vec3 getDirection() const;

        // Uhly kuzela (v radianoch)
        float innerCutoff;  // Vnutorny uhol (plna intenzita)
        float outerCutoff;  // Vonkajsi uhol (fade out)

        // Pomocna metoda pre nastavenie uhlov v stupnoch
        void setCutoff(float innerDegrees, float outerDegrees);

        // Attenuation (rovnake ako PointLight)
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;

        void setRange(float range);

        // Override
        void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) override;

    private:
        glm::vec3 position;
        glm::vec3 direction;
    };

} // namespace ppgso

#endif // PPGSO_SPOT_LIGHT_H