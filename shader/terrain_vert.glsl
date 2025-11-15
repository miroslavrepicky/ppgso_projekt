#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vNormal;
out vec3 vWorldPos;
out vec2 vUV;

void main() {
    vec4 world = modelMatrix * vec4(inPos, 1.0);
    vWorldPos = world.xyz;
    vNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
    vUV = inUV;
    gl_Position = projectionMatrix * viewMatrix * world;
}