#include "object.h"

namespace ppgso {

    Object::Object(const std::string& name) : SceneNode(name) {
    }

    Object::~Object() {
        // Cleanup
    }

    void Object::update(float deltaTime) {
        // Zakladna implementacia
        // Odvonene triedy mozu prepist
    }

    void Object::render() {
        // Zakladne renderovanie bez kamery
        // Tato metoda sa vola z SceneNode::renderRecursive()
        // Skutocne renderovanie je v renderWithCamera()
    }

    void Object::renderWithCamera(const Camera& camera) {
        if (!mesh || !shader) return;

        shader->use();
        setupShaderUniforms(camera);

        // WORKAROUND - compute transformed normals on CPU
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(transform.getWorldMatrix())));

        // Test - send one transformed normal vector
        glm::vec3 testNormal = normalMatrix * glm::vec3(0, 1, 0);
        shader->setUniform("testTransformedNormal", testNormal);

        std::cout << "Test normal: " << testNormal.x << ", " << testNormal.y << ", " << testNormal.z << std::endl;

        mesh->render();
    }

    // Helper metody
    void Object::loadMesh(const std::string& filename) {
        try {
            mesh = std::make_unique<ppgso::Mesh>(filename);
        } catch (std::exception& e) {
            std::cerr << "Error loading mesh " << filename << ": " << e.what() << std::endl;
        }
    }

    void Object::loadShader(const std::string& vertPath, const std::string& fragPath) {
        std::cout << "Vert shader length IN loadShader: " << vertPath.length() << std::endl;
        std::cout << "Frag shader length IN loadShader: " << fragPath.length() << std::endl;
        try {
            // ppgso::Shader očakáva cesty bez .glsl prípony alebo priamo string s kodom
            shader = std::make_unique<ppgso::Shader>(vertPath, fragPath);
        } catch (std::exception& e) {
            std::cerr << "Error loading shader " << vertPath << "/" << fragPath << ": " << e.what() << std::endl;
        }
    }

    void Object::loadShader(const char* vertCode, const char* fragCode) {
        try {
            // Priame nacitanie z string kodu (ako v test_cube.cpp)
            shader = std::make_unique<ppgso::Shader>(vertCode, fragCode);
        } catch (std::exception& e) {
            std::cerr << "Error loading shader from code: " << e.what() << std::endl;
        }
    }

    void Object::loadTexture(const std::string& filename) {
        try {
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(filename));
        } catch (std::exception& e) {
            std::cerr << "Error loading texture " << filename << ": " << e.what() << std::endl;
        }
    }

    void Object::setupShaderUniforms(const Camera& camera) {
        if (!shader) return;

        glm::mat4 modelMatrix = transform.getWorldMatrix();
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", camera.getViewMatrix());
        shader->setUniform("ProjectionMatrix", camera.getProjectionMatrix());

        // NormalMatrix workaround
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
        shader->setUniform("NormalMatrix", normalMatrix);
    }
} // namespace ppgso