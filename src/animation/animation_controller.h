#ifndef PPGSO_ANIMATION_CONTROLLER_H
#define PPGSO_ANIMATION_CONTROLLER_H

#include "keyframe.h"
#include "interpolation.h"
#include "../scene_graph/transform.h"
#include <memory>

namespace ppgso {

    /**
     * AnimationController - Riadi animaciu objektu pomocou keyframes
     */
    class AnimationController {
    public:
        AnimationController();

        // Nastavenie track
        void setTrack(std::shared_ptr<KeyframeTrack> track);
        std::shared_ptr<KeyframeTrack> getTrack() const;

        // Update animacie
        void update(float deltaTime);

        // Aplikovanie animacie na transform
        void applyToTransform(Transform& transform);

        // Playback control
        void play();
        void pause();
        void stop();
        void reset();

        bool isPlaying() const;
        bool isPaused() const;

        // Time control
        void setTime(float time);
        float getTime() const;
        float getDuration() const;

        // Speed control
        void setSpeed(float speed);
        float getSpeed() const;

        // Interpolation mode
        enum class InterpolationMode {
            LINEAR,
            SMOOTH,
            CATMULL_ROM,
            BEZIER
        };

        void setInterpolationMode(InterpolationMode mode);
        InterpolationMode getInterpolationMode() const;

    private:
        std::shared_ptr<KeyframeTrack> track;
        float currentTime;
        float speed;
        bool playing;
        bool paused;
        InterpolationMode interpolationMode;

        // Vypocita interpolovanu poziciu, rotaciu, skalu
        glm::vec3 interpolatePosition(float time) const;
        glm::quat interpolateRotation(float time) const;
        glm::vec3 interpolateScale(float time) const;
    };

} // namespace ppgso

#endif