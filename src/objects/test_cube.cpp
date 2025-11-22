#include "test_cube.h"
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <iostream>

namespace ppgso {

    TestCube::TestCube() : Object("TestCube") {
        loadMesh("cube.obj");

        loadShader(phong_vert_glsl, phong_frag_glsl);


        if (!shader) {
            std::cerr << "ERROR: Shader failed to load!" << std::endl;
        }

        setupMaterial();
        rotationSpeed = 1.0f;
        enableRotation = true;
    }

    void TestCube::setupMaterial() {
        materialAmbient = glm::vec3(0.8f, 0.6f, 0.2f);
        materialDiffuse = glm::vec3(0.8f, 0.6f, 0.2f);
        materialSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
        materialShininess = 32.0f;
    }

    void TestCube::update(float deltaTime) {
        if (enableRotation) {
            transform.rotate(deltaTime * rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
            transform.rotate(deltaTime * rotationSpeed * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }

    void TestCube::renderWithCamera(const Camera& camera) {
        if (!mesh || !shader) {
            std::cerr << "❌ Cannot render: mesh=" << (mesh ? "OK" : "NULL")
                      << ", shader=" << (shader ? "OK" : "NULL") << std::endl;
            return;
        }

        std::cout << "🎨 Rendering cube..." << std::endl;

        shader->use();
        setupShaderUniforms(camera);

        shader->setUniform("material.ambient", materialAmbient);
        shader->setUniform("material.diffuse", materialDiffuse);
        shader->setUniform("material.specular", materialSpecular);
        shader->setUniform("material.shininess", materialShininess);

        shader->setUniform("viewPos", camera.getPosition());
        shader->setUniform("useTexture", false);
        shader->setUniform("useBlinnPhong", true);

        int numLights = std::min((int)lights.size(), 3);
        shader->setUniform("numLights", numLights);

        for (int i = 0; i < numLights; i++) {
            if (lights[i]) {
                lights[i]->setupShaderUniforms(*shader, i);
            }
        }

        std::cout << "📦 Calling mesh->render()..." << std::endl;  // ✅ Vidíte toto?
        mesh->render();
        std::cout << "✅ mesh->render() done" << std::endl;  // ✅ Vidíte toto?
    }

    void TestCube::setLights(const std::vector<std::shared_ptr<Light>>& lights) {
        this->lights = lights;
    }

} // namespace ppgso