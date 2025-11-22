#include "animation_controller.h"

namespace ppgso {

    AnimationController::AnimationController()
        : currentTime(0.0f)
        , speed(1.0f)
        , playing(false)
        , paused(false)
        , interpolationMode(InterpolationMode::CATMULL_ROM)
    {
    }

    void AnimationController::setTrack(std::shared_ptr<KeyframeTrack> track) {
        this->track = track;
        reset();
    }

    std::shared_ptr<KeyframeTrack> AnimationController::getTrack() const {
        return track;
    }

    void AnimationController::update(float deltaTime) {
        if (!playing || paused || !track || track->isEmpty()) {
            return;
        }

        currentTime += deltaTime * speed;

        // Loop handling
        if (track->isLoop() && currentTime > getDuration()) {
            currentTime = fmod(currentTime, getDuration());
        }

        // Stop at end if not looping
        if (!track->isLoop() && currentTime >= getDuration()) {
            currentTime = getDuration();
            stop();
        }
    }

    void AnimationController::applyToTransform(Transform& transform) {
        if (!track || track->isEmpty()) return;

        transform.setPosition(interpolatePosition(currentTime));
        transform.setRotation(interpolateRotation(currentTime));
        transform.setScale(interpolateScale(currentTime));
    }

    void AnimationController::play() {
        playing = true;
        paused = false;
    }

    void AnimationController::pause() {
        paused = true;
    }

    void AnimationController::stop() {
        playing = false;
        paused = false;
        currentTime = 0.0f;
    }

    void AnimationController::reset() {
        currentTime = 0.0f;
        paused = false;
    }

    bool AnimationController::isPlaying() const {
        return playing && !paused;
    }

    bool AnimationController::isPaused() const {
        return paused;
    }

    void AnimationController::setTime(float time) {
        currentTime = time;
    }

    float AnimationController::getTime() const {
        return currentTime;
    }

    float AnimationController::getDuration() const {
        return track ? track->getDuration() : 0.0f;
    }

    void AnimationController::setSpeed(float speed) {
        this->speed = speed;
    }

    float AnimationController::getSpeed() const {
        return speed;
    }

    void AnimationController::setInterpolationMode(InterpolationMode mode) {
        this->interpolationMode = mode;
    }

    AnimationController::InterpolationMode AnimationController::getInterpolationMode() const {
        return interpolationMode;
    }

    glm::vec3 AnimationController::interpolatePosition(float time) const {
        if (!track || track->isEmpty()) return glm::vec3(0.0f);

        int idx0, idx1;
        float t;
        track->getKeyframesAtTime(time, idx0, idx1, t);

        if (idx0 < 0 || idx1 < 0) return glm::vec3(0.0f);

        const auto& keyframes = track->getKeyframes();
        const auto& kf0 = keyframes[idx0];
        const auto& kf1 = keyframes[idx1];

        switch (interpolationMode) {
            case InterpolationMode::LINEAR:
                return Interpolation::lerp(kf0.position, kf1.position, t);

            case InterpolationMode::SMOOTH:
                return Interpolation::lerp(kf0.position, kf1.position, Interpolation::smootherstep(t));

            case InterpolationMode::CATMULL_ROM: {
                // Pre Catmull-Rom potrebujeme 4 body
                if (keyframes.size() < 4) {
                    return Interpolation::lerp(kf0.position, kf1.position, Interpolation::smootherstep(t));
                }

                int idxPrev = std::max(0, idx0 - 1);
                int idxNext = std::min((int)keyframes.size() - 1, idx1 + 1);

                return Interpolation::catmullRom(
                    keyframes[idxPrev].position,
                    kf0.position,
                    kf1.position,
                    keyframes[idxNext].position,
                    t
                );
            }

            case InterpolationMode::BEZIER: {
                // Jednoduchy Bezier s kontrolnymi bodmi
                glm::vec3 control1 = kf0.position + (kf1.position - kf0.position) * 0.33f;
                glm::vec3 control2 = kf0.position + (kf1.position - kf0.position) * 0.66f;
                return Interpolation::cubicBezier(kf0.position, control1, control2, kf1.position, t);
            }

            default:
                return Interpolation::lerp(kf0.position, kf1.position, t);
        }
    }

    glm::quat AnimationController::interpolateRotation(float time) const {
        if (!track || track->isEmpty()) return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        int idx0, idx1;
        float t;
        track->getKeyframesAtTime(time, idx0, idx1, t);

        if (idx0 < 0 || idx1 < 0) return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        const auto& keyframes = track->getKeyframes();
        const auto& kf0 = keyframes[idx0];
        const auto& kf1 = keyframes[idx1];

        // Pre rotacie vzdy pouzijeme slerp
        float smoothT = (interpolationMode == InterpolationMode::SMOOTH || 
                        interpolationMode == InterpolationMode::CATMULL_ROM) 
                        ? Interpolation::smootherstep(t) : t;

        return Interpolation::slerp(kf0.rotation, kf1.rotation, smoothT);
    }

    glm::vec3 AnimationController::interpolateScale(float time) const {
        if (!track || track->isEmpty()) return glm::vec3(1.0f);

        int idx0, idx1;
        float t;
        track->getKeyframesAtTime(time, idx0, idx1, t);

        if (idx0 < 0 || idx1 < 0) return glm::vec3(1.0f);

        const auto& keyframes = track->getKeyframes();
        const auto& kf0 = keyframes[idx0];
        const auto& kf1 = keyframes[idx1];

        float smoothT = (interpolationMode == InterpolationMode::SMOOTH) 
                        ? Interpolation::smootherstep(t) : t;

        return Interpolation::lerp(kf0.scale, kf1.scale, smoothT);
    }

} // namespace ppgso