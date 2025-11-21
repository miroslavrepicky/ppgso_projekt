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
        if (!mesh || !shader) {
            return;
        }

        // Pouzit shader
        shader->use();

        // Nastavit uniformy
        setupShaderUniforms(camera);

        // Bind texture ak existuje
        if (texture) {
            shader->setUniform("Texture", *texture);
        }

        // Vykreslit mesh
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

    void Object::loadShader(const std::string& vertShaderSrc, const std::string& fragShaderSrc) {
        try {
            // Priamo pouzi embedded shader stringy (nie cesty k suborom)
            shader = std::make_unique<ppgso::Shader>(vertShaderSrc, fragShaderSrc);
        } catch (std::exception& e) {
            std::cerr << "Error loading shader: " << e.what() << std::endl;
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

        // Model matica (world transform)
        glm::mat4 modelMatrix = transform.getWorldMatrix();
        shader->setUniform("ModelMatrix", modelMatrix);

        // View matica
        glm::mat4 viewMatrix = camera.getViewMatrix();
        shader->setUniform("ViewMatrix", viewMatrix);

        // Projection matica
        glm::mat4 projectionMatrix = camera.getProjectionMatrix();
        shader->setUniform("ProjectionMatrix", projectionMatrix);

        // ModelViewProjection matica (pre optimalizaciu)
        glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
        shader->setUniform("MVP", mvp);

        // Normal matica (pre osvetlenie)
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
        shader->setUniform("NormalMatrix", normalMatrix);
    }

} // namespace ppgso