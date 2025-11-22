#ifndef PPGSO_LIGHT_H
#define PPGSO_LIGHT_H

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include <string>

namespace ppgso {

    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class Light {
    public:
        Light(LightType type);
        virtual ~Light();

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float intensity;
        bool enabled;

        LightType getType() const;
        virtual void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) = 0;

        void setColor(const glm::vec3& color);

    protected:
        LightType type;
        std::string getUniformName(int lightIndex, const std::string& property) const;
    };

} // namespace ppgso

#endif