#ifndef PPGSO_LIGHT_H
#define PPGSO_LIGHT_H

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include <string>

namespace ppgso {

    /**
     * Typy svetiel
     */
    enum class LightType {
        DIRECTIONAL,  // Smerove svetlo (slnko)
        POINT,        // Bodove svetlo
        SPOT          // Reflektor
    };

    /**
     * Light - Abstraktna baza pre vsetky svetla
     * Podporuje ambient, diffuse a specular zlozky
     */
    class Light {
    public:
        Light(LightType type);
        virtual ~Light();

        // Farby svetla (HDR - mozu byt > 1.0)
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        // Intenzita
        float intensity;

        // Zapnute/vypnute
        bool enabled;

        // Typ svetla
        LightType getType() const;

        // Nastavenie uniformov v shaderi
        virtual void setupShaderUniforms(ppgso::Shader& shader, int lightIndex) = 0;

        // Helper metody pre farby
        void setColor(const glm::vec3& color);
        void setAmbient(const glm::vec3& ambient);
        void setDiffuse(const glm::vec3& diffuse);
        void setSpecular(const glm::vec3& specular);

    protected:
        LightType type;

        // Helper pre nastavenie uniformov
        std::string getUniformName(int lightIndex, const std::string& property) const;
    };

} // namespace ppgso

#endif // PPGSO_LIGHT_H