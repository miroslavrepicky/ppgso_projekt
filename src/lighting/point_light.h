#ifndef PPGSO_POINT_LIGHT_H
#define PPGSO_POINT_LIGHT_H

#include "light.h"

namespace ppgso {

    class PointLight : public Light {
    public:
        PointLight();
        PointLight(const glm::vec3& position);

        void setPosition(const glm::vec3& position);
        glm::vec3 getPosition() const;

        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;

        void setRange(float range);
        void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) override;

    private:
        glm::vec3 position;
    };

} // namespace ppgso

#endif