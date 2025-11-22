#ifndef PPGSO_INTERPOLATION_H
#define PPGSO_INTERPOLATION_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ppgso {

    /**
     * Interpolation - Rôzne interpolačné funkcie pre plynulé animácie
     */
    namespace Interpolation {

        // Linearna interpolacia
        float lerp(float a, float b, float t);
        glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t);
        glm::quat slerp(const glm::quat& a, const glm::quat& b, float t);

        // Smoothstep (smooth ease-in-out)
        float smoothstep(float t);
        float smootherstep(float t);

        // Catmull-Rom spline (komplexna krivka)
        glm::vec3 catmullRom(const glm::vec3& p0, const glm::vec3& p1, 
                              const glm::vec3& p2, const glm::vec3& p3, float t);

        // Cubic Hermite spline
        glm::vec3 cubicHermite(const glm::vec3& p0, const glm::vec3& m0,
                                const glm::vec3& p1, const glm::vec3& m1, float t);

        // Ease funkcie
        float easeInQuad(float t);
        float easeOutQuad(float t);
        float easeInOutQuad(float t);
        
        float easeInCubic(float t);
        float easeOutCubic(float t);
        float easeInOutCubic(float t);

        // Bezier curve (cubic)
        glm::vec3 cubicBezier(const glm::vec3& p0, const glm::vec3& p1,
                               const glm::vec3& p2, const glm::vec3& p3, float t);
    }

} // namespace ppgso

#endif