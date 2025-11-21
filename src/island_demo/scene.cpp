#include "scene.h"

namespace ppgso {

    Scene::Scene()
        : time(0.0f)
        , width(800)
        , height(600)
    {
        // Vytvor root uzol grafu sceny
        rootNode = std::make_shared<SceneNode>("Root");
    }

    Scene::~Scene() {
        // Cleanup
    }

    void Scene::initialize() {
        // Nastav kameru
        camera.setPerspective(
            glm::radians(45.0f),  // FOV
            (float)width / height, // Aspect ratio
            0.1f,                  // Near plane
            1000.0f                // Far plane
        );

        // Nastav poziciu kamery
        camera.setPosition(glm::vec3(0.0f, 10.0f, 30.0f));
        camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        // Setup sceny
        setupScene();
        setupLights();
        setupObjects();
    }

    void Scene::update(float deltaTime) {
        time += deltaTime;

        // Update kamery
        camera.update(deltaTime);

        // Update grafu sceny (rekurzivne)
        rootNode->updateRecursive(deltaTime);
    }

    void Scene::render() {
        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Render graf sceny (rekurzivne)
        rootNode->renderRecursive();
    }

    void Scene::resize(int width, int height) {
        this->width = width;
        this->height = height;

        // Update aspect ratio kamery
        camera.setAspectRatio((float)width / height);

        // Update viewport
        glViewport(0, 0, width, height);
    }

    // Kamera
    Camera& Scene::getCamera() {
        return camera;
    }

    const Camera& Scene::getCamera() const {
        return camera;
    }

    // Root node
    std::shared_ptr<SceneNode> Scene::getRootNode() {
        return rootNode;
    }

    // Pomocne metody
    void Scene::addNode(std::shared_ptr<SceneNode> node) {
        if (node) {
            rootNode->addChild(node);
        }
    }

    void Scene::removeNode(std::shared_ptr<SceneNode> node) {
        if (node) {
            rootNode->removeChild(node);
        }
    }

    float Scene::getTime() const {
        return time;
    }

    // Setup metody
    void Scene::setupScene() {
        // Nastav background color (ocean blue)
        glClearColor(0.1f, 0.3f, 0.5f, 1.0f);

        // Enable blending pre priesvitne objekty (neskorsie)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Scene::setupLights() {
        // TODO: Implementovat v dalsej faze
        // Vytvorit smerove svetlo (slnko)
        // Vytvorit bodove svetla
        // Vytvorit reflektory
    }

    void Scene::setupObjects() {
        // TODO: Implementovat v dalsej faze
        // Vytvorit ostrov
        // Vytvorit stromy
        // Vytvorit skaly
        // atd.
    }

} // namespace ppgso