#ifndef PPGSO_CAMERA_PATH_H
#define PPGSO_CAMERA_PATH_H

#include "camera.h"
#include "../animation/keyframe.h"
#include "../animation/animation_controller.h"
#include <memory>

namespace ppgso {

    /**
     * CameraKeyframe - Keyframe pre kameru (pozicia + target)
     */
    struct CameraKeyframe {
        float time;
        glm::vec3 position;
        glm::vec3 target;      // Kam sa kamera pozera
        glm::vec3 up;          // Up vector

        CameraKeyframe();
        CameraKeyframe(float time, const glm::vec3& position, const glm::vec3& target);
        CameraKeyframe(float time, const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);
    };

    /**
     * CameraPath - Animovana cesta kamery
     */
    class CameraPath {
    public:
        CameraPath();

        // Pridanie keyframes
        void addKeyframe(const CameraKeyframe& keyframe);
        void addKeyframe(float time, const glm::vec3& position, const glm::vec3& target);

        // Update
        void update(float deltaTime);

        // Aplikovanie na kameru
        void applyToCamera(Camera& camera);

        // Playback control
        void play();
        void pause();
        void stop();
        void reset();

        bool isPlaying() const;

        // Time control
        void setTime(float time);
        float getTime() const;
        float getDuration() const;

        // Speed
        void setSpeed(float speed);
        float getSpeed() const;

        // Loop
        void setLoop(bool loop);
        bool isLoop() const;

        // Interpolation mode
        void setInterpolationMode(AnimationController::InterpolationMode mode);

    private:
        std::vector<CameraKeyframe> keyframes;
        float currentTime;
        float speed;
        bool playing;
        bool paused;
        bool loop;
        AnimationController::InterpolationMode interpolationMode;

        // Interpolacia
        glm::vec3 interpolatePosition(float time) const;
        glm::vec3 interpolateTarget(float time) const;
        
        void getKeyframesAtTime(float time, int& idx0, int& idx1, float& t) const;
    };

} // namespace ppgso

#endif