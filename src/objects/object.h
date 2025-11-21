#ifndef PPGSO_OBJECT_H
#define PPGSO_OBJECT_H

#include <memory>
#include <ppgso/ppgso.h>

#include "../scene_graph/scene_node.h"
#include "../camera/camera.h"

namespace ppgso {

    /**
     * Object - Abstraktna baza pre vsetky objekty v scene
     * Roziruje SceneNode o renderovanie s meshom a shadrom
     */
    class Object : public SceneNode {
    public:
        Object(const std::string& name = "Object");
        virtual ~Object();

        // Override SceneNode metod
        void update(float deltaTime) override;
        void render() override;

        // Rendering s kamerou
        virtual void renderWithCamera(const Camera& camera);

    protected:
        // Mesh a shader (budu inicializovane v odvodených triedach)
        std::unique_ptr<ppgso::Mesh> mesh;
        std::unique_ptr<ppgso::Shader> shader;
        std::unique_ptr<ppgso::Texture> texture;

        // Helper metody
        virtual void loadMesh(const std::string& filename);
        virtual void loadShader(const std::string& vertPath, const std::string& fragPath);
        virtual void loadTexture(const std::string& filename);

        // Rendering helpers
        virtual void setupShaderUniforms(const Camera& camera);
    };

} // namespace ppgso

#endif // PPGSO_OBJECT_H