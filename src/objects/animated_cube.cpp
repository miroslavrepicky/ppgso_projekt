#include "animated_cube.h"
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <iostream>

namespace ppgso {

    AnimatedCube::AnimatedCube() 
        : Object("AnimatedCube")
        , bobbingEnabled(false)
        , bobbingAmplitude(1.0f)
        , bobbingSpeed(2.0f)
        , bobbingTime(0.0f)
    {
        loadMesh("cube.obj");
        loadShader(phong_vert_glsl, phong_frag_glsl);

        setupMaterial();
        setupAnimation();
        
        basePosition = transform.getPosition();
    }

    void AnimatedCube::setupMaterial() {
        // Modra kocka
        materialAmbient = glm::vec3(0.2f, 0.4f, 0.8f);
        materialDiffuse = glm::vec3(0.3f, 0.6f, 1.0f);
        materialSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
        materialShininess = 32.0f;
    }

    void AnimatedCube::setupAnimation() {
        // Vytvor keyframe track
        auto track = std::make_shared<KeyframeTrack>();
        track->setName("CubeAnimation");
        track->setLoop(true);

        // Keyframes - kruhovy pohyb
        float radius = 5.0f;
        int numKeyframes = 8;
        
        for (int i = 0; i <= numKeyframes; i++) {
            float t = (float)i / numKeyframes;
            float angle = t * 2.0f * M_PI;
            float time = t * 10.0f; // 10 sekund cyklus
            
            glm::vec3 position(
                cos(angle) * radius,
                sin(angle * 2.0f) * 2.0f, // Hore/dole
                sin(angle) * radius
            );

            // Rotacia aby sa kocka otacala
            glm::quat rotation = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            
            track->addKeyframe(time, position, rotation);
        }

        animationController.setTrack(track);
        animationController.setInterpolationMode(AnimationController::InterpolationMode::CATMULL_ROM);
        animationController.play();
    }

    void AnimatedCube::update(float deltaTime) {
        // Update keyframe animacia
        animationController.update(deltaTime);
        animationController.applyToTransform(transform);

        // Proceduralna animacia - bobbing (hore/dole)
        if (bobbingEnabled) {
            bobbingTime += deltaTime * bobbingSpeed;
            float bobOffset = sin(bobbingTime) * bobbingAmplitude;
            
            glm::vec3 pos = transform.getPosition();
            pos.y += bobOffset;
            transform.setPosition(pos);
        }
    }

    void AnimatedCube::renderWithCamera(const Camera& camera) {
        if (!mesh || !shader) return;

        shader->use();
        setupShaderUniforms(camera);

        shader->setUniform("material.ambient", materialAmbient);
        shader->setUniform("material.diffuse", materialDiffuse);
        shader->setUniform("material.specular", materialSpecular);
        shader->setUniform("material.shininess", materialShininess);

        shader->setUniform("viewPos", camera.getPosition());
        shader->setUniform("useTexture", false);
        shader->setUniform("useBlinnPhong", true);

        int numLights = std::min((int)lights.size(), 3);
        shader->setUniform("numLights", numLights);

        for (int i = 0; i < numLights; i++) {
            if (lights[i]) {
                lights[i]->setupShaderUniforms(*shader, i);
            }
        }

        mesh->render();
    }

    void AnimatedCube::setLights(const std::vector<std::shared_ptr<Light>>& lights) {
        this->lights = lights;
    }

    AnimationController& AnimatedCube::getAnimationController() {
        return animationController;
    }

    void AnimatedCube::enableBobbing(bool enable) {
        bobbingEnabled = enable;
        if (enable) {
            basePosition = transform.getPosition();
        }
    }

    void AnimatedCube::setBobbingAmplitude(float amplitude) {
        bobbingAmplitude = amplitude;
    }

    void AnimatedCube::setBobbingSpeed(float speed) {
        bobbingSpeed = speed;
    }

} // namespace ppgso