//
// Created by PC on 9. 11. 2025.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ppgso.h>
#include <cmath>

int main() {
    // Create window
    ppgso::Window window{"My Project", 512, 512};

    // Main loop
    float time = 0.0f;
    while (window.pollEvents()) {
        // Animate background color
        time += 0.01f;
        float r = (sin(time) + 1.0f) / 2.0f;
        float g = (sin(time + 2.0f) + 1.0f) / 2.0f;
        float b = (sin(time + 4.0f) + 1.0f) / 2.0f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    return 0;
}