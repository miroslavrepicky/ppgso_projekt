#include "scene.h"
#include "objects/test_cube.h"
#include "objects/object.h"

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

        // DOLEZITE: Najprv vytvor svetla
        setupLights();

        // Potom objekty (potrebuju referenciu na svetla)
        setupObjects();

        std::cout << "Scene initialized with " << lights.size() << " lights" << std::endl;
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

        // Render vsetky objekty s kamerou
        renderNodeWithCamera(rootNode, camera);
    }

    void Scene::renderNodeWithCamera(std::shared_ptr<SceneNode> node, const Camera& camera) {
        if (!node || !node->isVisible()) return;

        // Ak je to Object, renderuj ho s kamerou
        auto obj = std::dynamic_pointer_cast<Object>(node);
        if (obj) {
            obj->renderWithCamera(camera);
        }

        // Renderuj deti
        for (auto& child : node->getChildren()) {
            renderNodeWithCamera(child, camera);
        }
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

    std::vector<std::shared_ptr<Light>>& Scene::getLights() {
        return lights;
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
        auto sun = std::make_shared<DirectionalLight>();
        sun->setDirection(glm::vec3(-0.3f, -1.0f, -0.5f));
        sun->ambient = glm::vec3(0.4f, 0.4f, 0.4f);
        sun->diffuse = glm::vec3(1.5f, 1.5f, 1.5f);
        sun->specular = glm::vec3(1.0f, 1.0f, 1.0f);
        sun->intensity = 1.0f;
        lights.push_back(sun);

        auto pointLight = std::make_shared<PointLight>(glm::vec3(5.0f, 10.0f, 5.0f));
        pointLight->setColor(glm::vec3(1.0f, 0.8f, 0.6f));
        pointLight->intensity = 2.0f;
        pointLight->setRange(50.0f);
        lights.push_back(pointLight);

        auto spotlight = std::make_shared<SpotLight>(
            glm::vec3(-5.0f, 10.0f, -5.0f),
            glm::vec3(0.5f, -1.0f, 0.5f)
        );
        spotlight->setColor(glm::vec3(0.8f, 0.9f, 1.0f));
        spotlight->intensity = 2.0f;
        spotlight->setCutoff(15.0f, 25.0f);
        spotlight->setRange(40.0f);
        lights.push_back(spotlight);

        std::cout << "Created " << lights.size() << " lights" << std::endl;
    }

    void Scene::setupObjects() {
        auto cube = std::make_shared<TestCube>();
        cube->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        cube->getTransform().setScale(2.0f);
        cube->setLights(lights);
        addNode(cube);

        std::cout << "Created test objects" << std::endl;
    }

}