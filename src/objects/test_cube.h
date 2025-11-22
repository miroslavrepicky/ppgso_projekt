#ifndef PPGSO_TEST_CUBE_H
#define PPGSO_TEST_CUBE_H

#include "../objects/object.h"
#include "../lighting/light.h"
#include <vector>
#include <memory>

namespace ppgso {

    class TestCube : public Object {
    public:
        TestCube();

        void update(float deltaTime) override;
        void renderWithCamera(const Camera& camera) override;

        glm::vec3 materialAmbient;
        glm::vec3 materialDiffuse;
        glm::vec3 materialSpecular;
        float materialShininess;

        void setLights(const std::vector<std::shared_ptr<Light>>& lights);

        float rotationSpeed;
        bool enableRotation;

    private:
        std::vector<std::shared_ptr<Light>> lights;
        void setupMaterial();
    };

} // namespace ppgso

#endif