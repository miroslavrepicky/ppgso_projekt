#ifndef PPGSO_SCENE_NODE_H
#define PPGSO_SCENE_NODE_H

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "transform.h"

namespace ppgso {

    // Forward declaration
    class Scene;

    /**
     * SceneNode - Uzol v grafe sceny
     * Podporuje stromovu hierarchiu s parent-child vztahmi
     * Kazdy uzol ma vlastnu transformaciu a moze mat viacero deti
     */
    class SceneNode {
    public:
        SceneNode(const std::string& name = "Node");
        virtual ~SceneNode();

        // Lifecycle methods
        virtual void update(float deltaTime);
        virtual void render();

        // Hierarchia
        void addChild(std::shared_ptr<SceneNode> child);
        void removeChild(std::shared_ptr<SceneNode> child);
        void setParent(SceneNode* parent);
        SceneNode* getParent() const;
        const std::vector<std::shared_ptr<SceneNode>>& getChildren() const;

        // Transform
        Transform& getTransform();
        const Transform& getTransform() const;

        // Identifikacia
        void setName(const std::string& name);
        std::string getName() const;

        // Viditelnost
        void setVisible(bool visible);
        bool isVisible() const;

        // Aktivita (ci sa vola update)
        void setActive(bool active);
        bool isActive() const;

        // Rekurzivne metody pre celu hierarchiu
        void updateRecursive(float deltaTime);
        void renderRecursive();

    protected:
        std::string name;
        Transform transform;
        SceneNode* parent;
        std::vector<std::shared_ptr<SceneNode>> children;
        bool visible;
        bool active;

        // Pomocne metody pre deti
        virtual void onChildAdded(std::shared_ptr<SceneNode> child);
        virtual void onChildRemoved(std::shared_ptr<SceneNode> child);
    };

} // namespace ppgso

#endif // PPGSO_SCENE_NODE_H