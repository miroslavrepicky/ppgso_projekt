#ifndef PPGSO_ANIMATED_CUBE_H
#define PPGSO_ANIMATED_CUBE_H

#include "../objects/object.h"
#include "../lighting/light.h"
#include "../animation/animation_controller.h"
#include <vector>
#include <memory>

namespace ppgso {

    /**
     * AnimatedCube - Kocka s keyframe animaciou
     * Demonštruje pouzitie AnimationController
     */
    class AnimatedCube : public Object {
    public:
        AnimatedCube();

        void update(float deltaTime) override;
        void renderWithCamera(const Camera& camera) override;

        // Material
        glm::vec3 materialAmbient;
        glm::vec3 materialDiffuse;
        glm::vec3 materialSpecular;
        float materialShininess;

        // Lights
        void setLights(const std::vector<std::shared_ptr<Light>>& lights);

        // Animation
        AnimationController& getAnimationController();
        void setupAnimation();

        // Proceduralna animacia (bobbing - hore/dole)
        void enableBobbing(bool enable);
        void setBobbingAmplitude(float amplitude);
        void setBobbingSpeed(float speed);

    private:
        std::vector<std::shared_ptr<Light>> lights;
        AnimationController animationController;
        
        // Proceduralna animacia
        bool bobbingEnabled;
        float bobbingAmplitude;
        float bobbingSpeed;
        float bobbingTime;
        glm::vec3 basePosition;

        void setupMaterial();
    };

} // namespace ppgso

#endif