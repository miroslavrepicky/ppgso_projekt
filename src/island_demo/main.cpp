#include <iostream>
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

/**
 * Island Demo - Graficke demo ostrova
 * 
 * Ovladanie:
 *   ESC - Ukoncit aplikaciu
 *   SPACE - Pozastavit/Pokracovat
 */
class IslandDemoWindow : public ppgso::Window {
private:
    std::unique_ptr<ppgso::Scene> scene;
    bool paused;
    float lastTime;

public:
    IslandDemoWindow() : Window{"Island Demo", WIDTH, HEIGHT}, paused(false), lastTime(0.0f) {
        // Inicializuj scenu
        scene = std::make_unique<ppgso::Scene>();
        scene->initialize();

        // Nastav FPS na 60
        glfwSwapInterval(1);
    }

    void onIdle() override {
        // Vypocitaj deltaTime
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Limit deltaTime (aby neboli velke skoky pri debugovani)
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        // Update sceny (ak nie je pozastavena)
        if (!paused) {
            scene->update(deltaTime);
        }

        // Render sceny
        scene->render();
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // ESC - ukoncit
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // SPACE - pause/unpause
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            paused = !paused;
            std::cout << (paused ? "Paused" : "Resumed") << std::endl;
        }
    }

    void onResize(int width, int height) {
        // Resize sceny a kamery
        scene->resize(width, height);
    }
};

int main() {
    // Vytvor a spusti okno
    try {
        IslandDemoWindow window;
        while (window.pollEvents()) {}
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}