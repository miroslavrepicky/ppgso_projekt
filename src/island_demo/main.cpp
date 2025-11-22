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
 *   1 - Zapnut/Vypnut smerove svetlo (slnko)
 *   2 - Zapnut/Vypnut bodove svetlo
 *   3 - Zapnut/Vypnut reflektor
 */
class IslandDemoWindow : public ppgso::Window {
private:
    std::unique_ptr<ppgso::Scene> scene;
    bool paused;
    float lastTime;

public:
    IslandDemoWindow() : Window{"Island Demo", WIDTH, HEIGHT}, paused(false), lastTime(0.0f) {
        // OpenGL nastavenia
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Inicializuj scenu
        scene = std::make_unique<ppgso::Scene>();
        scene->initialize();

        // Nastav FPS na 60
        glfwSwapInterval(1);

        std::cout << "==================================" << std::endl;
        std::cout << "  Island Demo - Phase 2" << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  ESC   - Quit" << std::endl;
        std::cout << "  SPACE - Pause/Resume" << std::endl;
        std::cout << "  1     - Toggle Directional Light" << std::endl;
        std::cout << "  2     - Toggle Point Light" << std::endl;
        std::cout << "  3     - Toggle Spot Light" << std::endl;
        std::cout << "==================================" << std::endl;
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
        std::cout << "KEY PRESSED: " << key << " action: " << action << std::endl;
        if (action != GLFW_PRESS) return;

        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Ukoncit aplikaciu
                glfwSetWindowShouldClose(window, true);
                break;

            case GLFW_KEY_SPACE:
                // Pause/unpause
                paused = !paused;
                std::cout << (paused ? "⏸ Paused" : "▶ Resumed") << std::endl;
                break;

            case GLFW_KEY_1:
                // Toggle directional light (slnko)
                if (!scene->getLights().empty()) {
                    auto& light = scene->getLights()[0];
                    light->enabled = !light->enabled;
                    std::cout << "☀ Directional Light: " << (light->enabled ? "ON" : "OFF") << std::endl;
                }else {
                    std::cout << "No lights available!" << std::endl;
                }
                break;

            case GLFW_KEY_2:
                // Toggle point light
                if (scene->getLights().size() > 1) {
                    auto& light = scene->getLights()[1];
                    light->enabled = !light->enabled;
                    std::cout << "💡 Point Light: " << (light->enabled ? "ON" : "OFF") << std::endl;
                }else {
                    std::cout << "No lights available!" << std::endl;
                }
                break;

            case GLFW_KEY_3:
                // Toggle spot light
                if (scene->getLights().size() > 2) {
                    auto& light = scene->getLights()[2];
                    light->enabled = !light->enabled;
                    std::cout << "🔦 Spot Light: " << (light->enabled ? "ON" : "OFF") << std::endl;
                }else {
                    std::cout << "No lights available!" << std::endl;
                }
                break;

            default:
                break;
        }
    }

    void onResize(int width, int height) {
        // Resize sceny a kamery
        scene->resize(width, height);
        std::cout << "Window resized to " << width << "x" << height << std::endl;
    }
};

int main() {
    std::cout << "=== PROGRAM START ===" << std::endl;

    try {
        IslandDemoWindow window;

        std::cout << "Starting main loop..." << std::endl;

        // EXPLICITNE volajte onIdle
        while (window.pollEvents()) {
            window.onIdle();  // PRIDAJTE TOTO
        }

        std::cout << "Main loop ended" << std::endl;

    } catch (std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "👋 Goodbye!" << std::endl;
    return EXIT_SUCCESS;
}