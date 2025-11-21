#include "test_cube.h"
#include <shaders/phong_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>

namespace ppgso {

    TestCube::TestCube() : Object("TestCube") {
        // Load mesh (ppgso ma built-in cube)
        loadMesh("cube.obj");
        
        // Load shader
        loadShader(phong_vert_glsl, phong_frag_glsl);
        
        // Setup material (zlaty kov)
        setupMaterial();

        
        // Rotacia
        rotationSpeed = 1.0f;
        enableRotation = true;
    }

    void TestCube::setupMaterial() {
        // Zlaty material
        materialAmbient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
        materialDiffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
        materialSpecular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
        materialShininess = 51.2f;
    }

    void TestCube::update(float deltaTime) {
        // Rotacia
        if (enableRotation) {
            transform.rotate(deltaTime * rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
            transform.rotate(deltaTime * rotationSpeed * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }

    void TestCube::renderWithCamera(const Camera& camera) {
        if (!mesh || !shader) return;

        // Use shader
        shader->use();

        // Setup basic uniforms
        setupShaderUniforms(camera);

        // Setup material uniforms
        shader->setUniform("material.ambient", materialAmbient);
        shader->setUniform("material.diffuse", materialDiffuse);
        shader->setUniform("material.specular", materialSpecular);
        shader->setUniform("material.shininess", materialShininess);

        // Camera position (for specular)
        shader->setUniform("viewPos", camera.getPosition());

        // Texture (zatial nepouzivame)
        shader->setUniform("useTexture", false);

        // Blinn-Phong (default true)
        shader->setUniform("useBlinnPhong", true);

        // Setup lights
        shader->setUniform("numLights", (int)lights.size());
        for (size_t i = 0; i < lights.size() && i < 10; i++) {
            if (lights[i]) {
                lights[i]->setupShaderUniforms(*shader, (int)i);
            }
        }

        // Render mesh
        mesh->render();

    }

    void TestCube::setLights(const std::vector<std::shared_ptr<Light>>& lights) {
        this->lights = lights;
    }

} // namespace ppgso