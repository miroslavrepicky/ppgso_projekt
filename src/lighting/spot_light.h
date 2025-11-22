#ifndef PPGSO_SPOT_LIGHT_H
#define PPGSO_SPOT_LIGHT_H

#include "light.h"

namespace ppgso {

    class SpotLight : public Light {
    public:
        SpotLight();
        SpotLight(const glm::vec3& position, const glm::vec3& direction);

        void setPosition(const glm::vec3& position);
        glm::vec3 getPosition() const;

        void setDirection(const glm::vec3& direction);
        glm::vec3 getDirection() const;

        float innerCutoff;
        float outerCutoff;

        void setCutoff(float innerDegrees, float outerDegrees);

        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;

        void setRange(float range);
        void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) override;

    private:
        glm::vec3 position;
        glm::vec3 direction;
    };

} // namespace ppgso

#endif