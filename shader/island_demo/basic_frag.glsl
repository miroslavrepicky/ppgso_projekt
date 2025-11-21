#version 330 core

// Input from vertex shader
in vec3 FragColor;
in vec2 TexCoord;

// Output
out vec4 color;

void main() {
    color = vec4(FragColor, 1.0);
}