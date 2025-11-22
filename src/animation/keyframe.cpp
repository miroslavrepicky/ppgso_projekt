#include "keyframe.h"
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>

namespace ppgso {

    // Keyframe
    Keyframe::Keyframe()
        : time(0.0f)
        , position(0.0f)
        , rotation(1.0f, 0.0f, 0.0f, 0.0f)
        , scale(1.0f)
    {
    }

    Keyframe::Keyframe(float time, const glm::vec3& position)
        : time(time)
        , position(position)
        , rotation(1.0f, 0.0f, 0.0f, 0.0f)
        , scale(1.0f)
    {
    }

    Keyframe::Keyframe(float time, const glm::vec3& position, const glm::quat& rotation)
        : time(time)
        , position(position)
        , rotation(rotation)
        , scale(1.0f)
    {
    }

    Keyframe::Keyframe(float time, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
        : time(time)
        , position(position)
        , rotation(rotation)
        , scale(scale)
    {
    }

    void Keyframe::setRotationEuler(const glm::vec3& eulerAngles) {
        rotation = glm::quat(eulerAngles);
    }

    // KeyframeTrack
    KeyframeTrack::KeyframeTrack()
        : loop(false)
        , name("Unnamed")
    {
    }

    void KeyframeTrack::addKeyframe(const Keyframe& keyframe) {
        keyframes.push_back(keyframe);
        
        // Zorad keyframes podla casu
        std::sort(keyframes.begin(), keyframes.end(), 
            [](const Keyframe& a, const Keyframe& b) {
                return a.time < b.time;
            });
    }

    void KeyframeTrack::addKeyframe(float time, const glm::vec3& position) {
        addKeyframe(Keyframe(time, position));
    }

    void KeyframeTrack::addKeyframe(float time, const glm::vec3& position, const glm::quat& rotation) {
        addKeyframe(Keyframe(time, position, rotation));
    }

    const std::vector<Keyframe>& KeyframeTrack::getKeyframes() const {
        return keyframes;
    }

    size_t KeyframeTrack::getKeyframeCount() const {
        return keyframes.size();
    }

    float KeyframeTrack::getDuration() const {
        if (keyframes.empty()) return 0.0f;
        return keyframes.back().time;
    }

    bool KeyframeTrack::isEmpty() const {
        return keyframes.empty();
    }

    void KeyframeTrack::getKeyframesAtTime(float time, int& keyIndex0, int& keyIndex1, float& t) const {
        if (keyframes.empty()) {
            keyIndex0 = keyIndex1 = -1;
            t = 0.0f;
            return;
        }

        // Loop handling
        if (loop && time > getDuration()) {
            time = fmod(time, getDuration());
        }

        // Clamp time
        if (time <= keyframes.front().time) {
            keyIndex0 = keyIndex1 = 0;
            t = 0.0f;
            return;
        }

        if (time >= keyframes.back().time) {
            keyIndex0 = keyIndex1 = (int)keyframes.size() - 1;
            t = 0.0f;
            return;
        }

        // Najdi keyframes
        for (size_t i = 0; i < keyframes.size() - 1; i++) {
            if (time >= keyframes[i].time && time <= keyframes[i + 1].time) {
                keyIndex0 = (int)i;
                keyIndex1 = (int)i + 1;
                
                float timeDiff = keyframes[i + 1].time - keyframes[i].time;
                t = (time - keyframes[i].time) / timeDiff;
                return;
            }
        }

        keyIndex0 = keyIndex1 = 0;
        t = 0.0f;
    }

    void KeyframeTrack::setLoop(bool loop) {
        this->loop = loop;
    }

    bool KeyframeTrack::isLoop() const {
        return loop;
    }

    void KeyframeTrack::setName(const std::string& name) {
        this->name = name;
    }

    std::string KeyframeTrack::getName() const {
        return name;
    }

} // namespace ppgso