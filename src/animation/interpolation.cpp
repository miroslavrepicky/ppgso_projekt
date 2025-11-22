#include "interpolation.h"
#include <algorithm>

namespace ppgso {
namespace Interpolation {

    // Linearna interpolacia
    float lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {
        return a + (b - a) * t;
    }

    glm::quat slerp(const glm::quat& a, const glm::quat& b, float t) {
        return glm::slerp(a, b, t);
    }

    // Smoothstep
    float smoothstep(float t) {
        t = glm::clamp(t, 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }

    float smootherstep(float t) {
        t = glm::clamp(t, 0.0f, 1.0f);
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    // Catmull-Rom spline
    glm::vec3 catmullRom(const glm::vec3& p0, const glm::vec3& p1,
                          const glm::vec3& p2, const glm::vec3& p3, float t) {
        float t2 = t * t;
        float t3 = t2 * t;

        glm::vec3 result = 
            0.5f * ((2.0f * p1) +
                    (-p0 + p2) * t +
                    (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
                    (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);

        return result;
    }

    // Cubic Hermite spline
    glm::vec3 cubicHermite(const glm::vec3& p0, const glm::vec3& m0,
                            const glm::vec3& p1, const glm::vec3& m1, float t) {
        float t2 = t * t;
        float t3 = t2 * t;

        float h00 = 2.0f * t3 - 3.0f * t2 + 1.0f;
        float h10 = t3 - 2.0f * t2 + t;
        float h01 = -2.0f * t3 + 3.0f * t2;
        float h11 = t3 - t2;

        return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
    }

    // Ease funkcie - Quad
    float easeInQuad(float t) {
        return t * t;
    }

    float easeOutQuad(float t) {
        return t * (2.0f - t);
    }

    float easeInOutQuad(float t) {
        return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
    }

    // Ease funkcie - Cubic
    float easeInCubic(float t) {
        return t * t * t;
    }

    float easeOutCubic(float t) {
        float f = t - 1.0f;
        return f * f * f + 1.0f;
    }

    float easeInOutCubic(float t) {
        return t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
    }

    // Bezier curve
    glm::vec3 cubicBezier(const glm::vec3& p0, const glm::vec3& p1,
                           const glm::vec3& p2, const glm::vec3& p3, float t) {
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        glm::vec3 result = uuu * p0;
        result += 3.0f * uu * t * p1;
        result += 3.0f * u * tt * p2;
        result += ttt * p3;

        return result;
    }

} // namespace Interpolation
} // namespace ppgso