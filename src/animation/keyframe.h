#ifndef PPGSO_KEYFRAME_H
#define PPGSO_KEYFRAME_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>

namespace ppgso {

    /**
     * Keyframe - Reprezentuje stav objektu v danom case
     * Obsahuje poziciu, rotaciu a skalu
     */
    struct Keyframe {
        float time;              // Cas v sekundach
        glm::vec3 position;      // Pozicia
        glm::quat rotation;      // Rotacia (quaternion)
        glm::vec3 scale;         // Skala

        Keyframe();
        Keyframe(float time, const glm::vec3& position);
        Keyframe(float time, const glm::vec3& position, const glm::quat& rotation);
        Keyframe(float time, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

        // Helper pre Euler angles
        void setRotationEuler(const glm::vec3& eulerAngles);
    };

    /**
     * KeyframeTrack - Kolekcia keyframes pre animaciu
     */
    class KeyframeTrack {
    public:
        KeyframeTrack();

        // Pridanie keyframe
        void addKeyframe(const Keyframe& keyframe);
        void addKeyframe(float time, const glm::vec3& position);
        void addKeyframe(float time, const glm::vec3& position, const glm::quat& rotation);

        // Ziskanie keyframes
        const std::vector<Keyframe>& getKeyframes() const;
        size_t getKeyframeCount() const;

        // Casove operacie
        float getDuration() const;
        bool isEmpty() const;

        // Ziskanie keyframes okolo daneho casu
        void getKeyframesAtTime(float time, int& keyIndex0, int& keyIndex1, float& t) const;

        // Nastavenia
        void setLoop(bool loop);
        bool isLoop() const;

        void setName(const std::string& name);
        std::string getName() const;

    private:
        std::vector<Keyframe> keyframes;
        bool loop;
        std::string name;
    };

} // namespace ppgso

#endif