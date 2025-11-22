#include "camera_path.h"
#include "../animation/interpolation.h"
#include <algorithm>

namespace ppgso {

    // CameraKeyframe
    CameraKeyframe::CameraKeyframe()
        : time(0.0f)
        , position(0.0f)
        , target(0.0f, 0.0f, -1.0f)
        , up(0.0f, 1.0f, 0.0f)
    {
    }

    CameraKeyframe::CameraKeyframe(float time, const glm::vec3& position, const glm::vec3& target)
        : time(time)
        , position(position)
        , target(target)
        , up(0.0f, 1.0f, 0.0f)
    {
    }

    CameraKeyframe::CameraKeyframe(float time, const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
        : time(time)
        , position(position)
        , target(target)
        , up(up)
    {
    }

    // CameraPath
    CameraPath::CameraPath()
        : currentTime(0.0f)
        , speed(1.0f)
        , playing(false)
        , paused(false)
        , loop(false)
        , interpolationMode(AnimationController::InterpolationMode::CATMULL_ROM)
    {
    }

    void CameraPath::addKeyframe(const CameraKeyframe& keyframe) {
        keyframes.push_back(keyframe);
        
        std::sort(keyframes.begin(), keyframes.end(),
            [](const CameraKeyframe& a, const CameraKeyframe& b) {
                return a.time < b.time;
            });
    }

    void CameraPath::addKeyframe(float time, const glm::vec3& position, const glm::vec3& target) {
        addKeyframe(CameraKeyframe(time, position, target));
    }

    void CameraPath::update(float deltaTime) {
        if (!playing || paused || keyframes.empty()) {
            return;
        }

        currentTime += deltaTime * speed;

        if (loop && currentTime > getDuration()) {
            currentTime = fmod(currentTime, getDuration());
        }

        if (!loop && currentTime >= getDuration()) {
            currentTime = getDuration();
            stop();
        }
    }

    void CameraPath::applyToCamera(Camera& camera) {
        if (keyframes.empty()) return;

        glm::vec3 position = interpolatePosition(currentTime);
        glm::vec3 target = interpolateTarget(currentTime);

        camera.setPosition(position);
        camera.lookAt(target);
    }

    void CameraPath::play() {
        playing = true;
        paused = false;
    }

    void CameraPath::pause() {
        paused = true;
    }

    void CameraPath::stop() {
        playing = false;
        paused = false;
        currentTime = 0.0f;
    }

    void CameraPath::reset() {
        currentTime = 0.0f;
        paused = false;
    }

    bool CameraPath::isPlaying() const {
        return playing && !paused;
    }

    void CameraPath::setTime(float time) {
        currentTime = time;
    }

    float CameraPath::getTime() const {
        return currentTime;
    }

    float CameraPath::getDuration() const {
        if (keyframes.empty()) return 0.0f;
        return keyframes.back().time;
    }

    void CameraPath::setSpeed(float speed) {
        this->speed = speed;
    }

    float CameraPath::getSpeed() const {
        return speed;
    }

    void CameraPath::setLoop(bool loop) {
        this->loop = loop;
    }

    bool CameraPath::isLoop() const {
        return loop;
    }

    void CameraPath::setInterpolationMode(AnimationController::InterpolationMode mode) {
        this->interpolationMode = mode;
    }

    glm::vec3 CameraPath::interpolatePosition(float time) const {
        if (keyframes.empty()) return glm::vec3(0.0f);

        int idx0, idx1;
        float t;
        getKeyframesAtTime(time, idx0, idx1, t);

        if (idx0 < 0 || idx1 < 0) return keyframes[0].position;

        const auto& kf0 = keyframes[idx0];
        const auto& kf1 = keyframes[idx1];

        switch (interpolationMode) {
            case AnimationController::InterpolationMode::CATMULL_ROM: {
                if (keyframes.size() < 4) {
                    return Interpolation::lerp(kf0.position, kf1.position, 
                                              Interpolation::smootherstep(t));
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

            case AnimationController::InterpolationMode::SMOOTH:
                return Interpolation::lerp(kf0.position, kf1.position, 
                                          Interpolation::smootherstep(t));

            default:
                return Interpolation::lerp(kf0.position, kf1.position, t);
        }
    }

    glm::vec3 CameraPath::interpolateTarget(float time) const {
        if (keyframes.empty()) return glm::vec3(0.0f, 0.0f, -1.0f);

        int idx0, idx1;
        float t;
        getKeyframesAtTime(time, idx0, idx1, t);

        if (idx0 < 0 || idx1 < 0) return keyframes[0].target;

        const auto& kf0 = keyframes[idx0];
        const auto& kf1 = keyframes[idx1];

        switch (interpolationMode) {
            case AnimationController::InterpolationMode::CATMULL_ROM: {
                if (keyframes.size() < 4) {
                    return Interpolation::lerp(kf0.target, kf1.target, 
                                              Interpolation::smootherstep(t));
                }

                int idxPrev = std::max(0, idx0 - 1);
                int idxNext = std::min((int)keyframes.size() - 1, idx1 + 1);

                return Interpolation::catmullRom(
                    keyframes[idxPrev].target,
                    kf0.target,
                    kf1.target,
                    keyframes[idxNext].target,
                    t
                );
            }

            case AnimationController::InterpolationMode::SMOOTH:
                return Interpolation::lerp(kf0.target, kf1.target, 
                                          Interpolation::smootherstep(t));

            default:
                return Interpolation::lerp(kf0.target, kf1.target, t);
        }
    }

    void CameraPath::getKeyframesAtTime(float time, int& idx0, int& idx1, float& t) const {
        if (keyframes.empty()) {
            idx0 = idx1 = -1;
            t = 0.0f;
            return;
        }

        if (time <= keyframes.front().time) {
            idx0 = idx1 = 0;
            t = 0.0f;
            return;
        }

        if (time >= keyframes.back().time) {
            idx0 = idx1 = (int)keyframes.size() - 1;
            t = 0.0f;
            return;
        }

        for (size_t i = 0; i < keyframes.size() - 1; i++) {
            if (time >= keyframes[i].time && time <= keyframes[i + 1].time) {
                idx0 = (int)i;
                idx1 = (int)i + 1;
                
                float timeDiff = keyframes[i + 1].time - keyframes[i].time;
                t = (time - keyframes[i].time) / timeDiff;
                return;
            }
        }

        idx0 = idx1 = 0;
        t = 0.0f;
    }

} // namespace ppgso