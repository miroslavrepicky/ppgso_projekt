#include "scene_node.h"
#include <algorithm>

namespace ppgso {

    SceneNode::SceneNode(const std::string& name)
        : name(name)
        , parent(nullptr)
        , visible(true)
        , active(true)
    {
    }

    SceneNode::~SceneNode() {
        // Cleanup children
        children.clear();
    }

    void SceneNode::update(float deltaTime) {
        // Zakladna implementacia - prazdna
        // Odvonene triedy prepisu tuto metodu
    }

    void SceneNode::render() {
        // Zakladna implementacia - prazdna
        // Odvonene triedy prepisu tuto metodu
    }

    // Hierarchia
    void SceneNode::addChild(std::shared_ptr<SceneNode> child) {
        if (!child) return;

        // Odstran z povodneho parenta ak existuje
        if (child->parent != nullptr) {
            child->parent->removeChild(child);
        }

        // Pridaj do children
        children.push_back(child);
        child->parent = this;

        // Nastav hierarchicku transformaciu
        child->transform.setParent(&this->transform);

        onChildAdded(child);
    }

    void SceneNode::removeChild(std::shared_ptr<SceneNode> child) {
        if (!child) return;

        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end()) {
            child->parent = nullptr;
            child->transform.setParent(nullptr);
            onChildRemoved(child);
            children.erase(it);
        }
    }

    void SceneNode::setParent(SceneNode* parent) {
        if (this->parent == parent) return;

        // Odstran zo stareho parenta
        if (this->parent != nullptr) {
            // Musime najst shared_ptr na seba
            auto& siblings = this->parent->children;
            for (auto it = siblings.begin(); it != siblings.end(); ++it) {
                if (it->get() == this) {
                    this->parent->removeChild(*it);
                    break;
                }
            }
        }

        // Nastav noveho parenta (ak nie je nullptr, musi byt pridany cez addChild)
        this->parent = parent;
        if (parent != nullptr) {
            transform.setParent(&parent->transform);
        } else {
            transform.setParent(nullptr);
        }
    }

    SceneNode* SceneNode::getParent() const {
        return parent;
    }

    const std::vector<std::shared_ptr<SceneNode>>& SceneNode::getChildren() const {
        return children;
    }

    // Transform
    Transform& SceneNode::getTransform() {
        return transform;
    }

    const Transform& SceneNode::getTransform() const {
        return transform;
    }

    // Identifikacia
    void SceneNode::setName(const std::string& name) {
        this->name = name;
    }

    std::string SceneNode::getName() const {
        return name;
    }

    // Viditelnost
    void SceneNode::setVisible(bool visible) {
        this->visible = visible;
    }

    bool SceneNode::isVisible() const {
        return visible;
    }

    // Aktivita
    void SceneNode::setActive(bool active) {
        this->active = active;
    }

    bool SceneNode::isActive() const {
        return active;
    }

    // Rekurzivne metody
    void SceneNode::updateRecursive(float deltaTime) {
        if (active) {
            update(deltaTime);

            // Update vsetkych deti
            for (auto& child : children) {
                child->updateRecursive(deltaTime);
            }
        }
    }

    void SceneNode::renderRecursive() {
        if (visible) {
            render();

            // Render vsetkych deti
            for (auto& child : children) {
                child->renderRecursive();
            }
        }
    }

    // Protected methods
    void SceneNode::onChildAdded(std::shared_ptr<SceneNode> child) {
        // Hook pre odvonene triedy
    }

    void SceneNode::onChildRemoved(std::shared_ptr<SceneNode> child) {
        // Hook pre odvonene triedy
    }

} // namespace ppgso