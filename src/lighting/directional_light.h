#ifndef PPGSO_DIRECTIONAL_LIGHT_H
#define PPGSO_DIRECTIONAL_LIGHT_H

#include "light.h"

namespace ppgso {

    class DirectionalLight : public Light {
    public:
        DirectionalLight();
        DirectionalLight(const glm::vec3& direction);

        void setDirection(const glm::vec3& direction);
        glm::vec3 getDirection() const;

        void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) override;

    private:
        glm::vec3 direction;
    };

} // namespace ppgso

#endif