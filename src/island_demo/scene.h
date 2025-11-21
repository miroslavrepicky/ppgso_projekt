#ifndef PPGSO_SCENE_H
#define PPGSO_SCENE_H

#include <memory>
#include <vector>
#include <ppgso/ppgso.h>

#include "camera/camera.h"
#include "scene_graph/scene_node.h"
#include "lighting/light.h"
#include "lighting/directional_light.h"
#include "lighting/point_light.h"
#include "lighting/spot_light.h"

namespace ppgso {

    /**
     * Scene - Hlavna scena aplikacie
     * Spravuje kameru, graf sceny a rendering
     */
    class Scene {
    public:
        Scene();
        ~Scene();

        // Inicializacia sceny
        void initialize();

        // Update sceny
        void update(float deltaTime);

        // Rendering sceny
        void render();

        // Resize (pri zmene velkosti okna)
        void resize(int width, int height);

        // Kamera
        Camera& getCamera();
        const Camera& getCamera() const;

        // Root uzol grafu sceny
        std::shared_ptr<SceneNode> getRootNode();

        // Pomocne metody pre pridavanie objektov
        void addNode(std::shared_ptr<SceneNode> node);
        void removeNode(std::shared_ptr<SceneNode> node);

        // Svetla - pristup k svetlam v scene
        std::vector<std::shared_ptr<Light>>& getLights();

        // Cas
        float getTime() const;

    private:
        // Kamera
        Camera camera;

        // Graf sceny
        std::shared_ptr<SceneNode> rootNode;

        // Svetla
        std::vector<std::shared_ptr<Light>> lights;

        // Cas
        float time;

        // Rozmery okna
        int width;
        int height;

        // Pomocne metody
        void setupScene();
        void setupLights();
        void setupObjects();

        // Helper pre rendering s kamerou
        void renderNodeWithCamera(std::shared_ptr<SceneNode> node, const Camera& camera);
    };

} // namespace ppgso

#endif // PPGSO_SCENE_H