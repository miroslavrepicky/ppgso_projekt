#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <ppgso/ppgso.h>
#include "terrain_vert_glsl.h"
#include "terrain_frag_glsl.h"





#include "terrain_vert_glsl.h"
#include "terrain_frag_glsl.h"

Terrain::Terrain(int resolution, float size, float height)
    : resolution(resolution), size(size), maxHeight(height),
      shader(terrain_vert_glsl, terrain_frag_glsl) {  // <-- tu
    generateGrid();
    computeNormals();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Positions
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3),
                 positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Normals
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // UV
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2),
                 uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Indices
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    indexCount = indices.size();


}


float Terrain::noise(float x, float y) {
    return 0.5f * (sinf(x*3.1f) + cosf(y*4.3f));
}

float Terrain::islandHeight(float x, float y) {
    float nx = x / size * 2.f - 1.f;
    float ny = y / size * 2.f - 1.f;

    float d = sqrt(nx*nx + ny*ny);
    float islandMask = glm::clamp(1.f - d, 0.f, 1.f);

    float h =
        0.6f * noise(x*0.1f, y*0.1f) +
        0.25f * noise(x*0.3f, y*0.3f) +
        0.1f * noise(x*1.0f, y*1.0f);

    h = glm::pow(glm::max(h, 0.f), 1.5f);
    return h * islandMask * maxHeight;
}

void Terrain::generateGrid() {
    positions.clear();
    uvs.clear();
    indices.clear();

    for(int z = 0; z <= resolution; z++) {
        for(int x = 0; x <= resolution; x++) {
            float fx = (float)x / resolution;
            float fz = (float)z / resolution;
            float wx = (fx - 0.5f) * size;
            float wz = (fz - 0.5f) * size;
            float wy = islandHeight(wx, wz);

            positions.push_back({wx, wy, wz});
            uvs.push_back({fx, fz});
        }
    }

    for (int z = 0; z < resolution; z++) {
        for (int x = 0; x < resolution; x++) {
            int i0 = z * (resolution + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + (resolution + 1);
            int i3 = i2 + 1;

            indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
            indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
        }
    }
}

void Terrain::computeNormals() {
    normals.resize(positions.size(), glm::vec3(0));

    for(size_t i = 0; i < indices.size(); i += 3) {
        int i0 = indices[i+0];
        int i1 = indices[i+1];
        int i2 = indices[i+2];

        glm::vec3 n = glm::normalize(glm::cross(
            positions[i1] - positions[i0],
            positions[i2] - positions[i0]
        ));

        normals[i0] += n;
        normals[i1] += n;
        normals[i2] += n;
    }

    for(auto &n : normals)
        n = glm::normalize(n);
}

void Terrain::update(float dt) {}

void Terrain::render(const glm::mat4 &view, const glm::mat4 &projection) {
    shader.use();
    shader.setUniform("modelMatrix", glm::mat4(1.0f));
    shader.setUniform("viewMatrix", view);
    shader.setUniform("projectionMatrix", projection);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}
