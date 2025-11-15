#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>  // obsahuje aj glad, glm, stb_image


class Terrain {
public:
    Terrain(int resolution = 256, float size = 80.0f, float height = 12.0f);

    void update(float dt);
    void render(const glm::mat4 &view, const glm::mat4 &projection);

private:
    int resolution;
    float size;
    float maxHeight;

    GLuint vao, vbo, nbo, tbo, ebo;
    size_t indexCount;

    ppgso::Shader shader;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indices;

    float noise(float x, float y);
    float islandHeight(float x, float y);

    void generateGrid();
    void computeNormals();
};
