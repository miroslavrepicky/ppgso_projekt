#include "test_cube.h"
#include <shaders/phong_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>

namespace ppgso {

    TestCube::TestCube() : Object("TestCube") {
        // Load mesh (ppgso ma built-in cube)
        loadMesh("cube.obj");

        // Load shader z generovanych headerov
        loadShader(phong_vert_glsl, phong_frag_glsl);

        if (!shader) {
            std::cerr << "FATAL: Shader failed to load!" << std::endl;
        } else {
            std::cout << "Shader loaded successfully" << std::endl;
        }

        // Setup material (zlaty kov)
        setupMaterial();

        // Rotacia
        rotationSpeed = 1.0f;
        enableRotation = true;
    }

    void TestCube::setupMaterial() {
        // Jednoduchsi material pre testovanie
        materialAmbient = glm::vec3(0.8f, 0.6f, 0.2f);   // Svetlejsi ambient!
        materialDiffuse = glm::vec3(0.8f, 0.6f, 0.2f);
        materialSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
        materialShininess = 32.0f;

        std::cout << "Material Ambient: " << materialAmbient.x << ", "
                  << materialAmbient.y << ", " << materialAmbient.z << std::endl;
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

        shader->use();
        setupShaderUniforms(camera);

        // Setup material uniforms
        shader->setUniform("material.ambient", materialAmbient);
        shader->setUniform("material.diffuse", materialDiffuse);
        shader->setUniform("material.specular", materialSpecular);
        shader->setUniform("material.shininess", materialShininess);

        shader->setUniform("viewPos", camera.getPosition());
        shader->setUniform("useTexture", false);
        shader->setUniform("useBlinnPhong", true);

        // Setup lights
        int numLights = (int)lights.size();
        shader->setUniform("numLights", numLights);

        // DEBUG - raz za cas
        static int debugCounter = 0;
        if (debugCounter++ % 120 == 0) {
            std::cout << "\n=== RENDER DEBUG ===" << std::endl;
            std::cout << "NumLights: " << numLights << std::endl;
            std::cout << "Material diffuse: " << materialDiffuse.x << ", "
                      << materialDiffuse.y << ", " << materialDiffuse.z << std::endl;
            std::cout << "Camera position: " << camera.getPosition().x << ", "
                      << camera.getPosition().y << ", " << camera.getPosition().z << std::endl;
        }

        for (size_t i = 0; i < lights.size() && i < 3; i++) {  // Max 3 svetla
            if (lights[i]) {
                if (debugCounter % 120 == 1) {
                    std::cout << "\nLight " << i << ":" << std::endl;
                    std::cout << "  Type: " << (int)lights[i]->getType() << std::endl;
                    std::cout << "  Enabled: " << lights[i]->enabled << std::endl;
                    std::cout << "  Intensity: " << lights[i]->intensity << std::endl;
                    std::cout << "  Ambient: " << lights[i]->ambient.x << ", "
                              << lights[i]->ambient.y << ", " << lights[i]->ambient.z << std::endl;
                    std::cout << "  Diffuse: " << lights[i]->diffuse.x << ", "
                              << lights[i]->diffuse.y << ", " << lights[i]->diffuse.z << std::endl;
                    std::cout << "  Specular: " << lights[i]->specular.x << ", "
                              << lights[i]->specular.y << ", " << lights[i]->specular.z << std::endl;
                }
                lights[i]->setupShaderUniforms(*shader, (int)i);
            }
        }

        mesh->render();
    }

    void TestCube::setLights(const std::vector<std::shared_ptr<Light>>& lights) {
        this->lights = lights;
    }

} // namespace ppgso