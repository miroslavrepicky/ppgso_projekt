#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// Output to fragment shader
out vec3 FragColor;
out vec2 TexCoord;

// Uniforms
uniform mat4 MVP;  // ModelViewProjection matrix

void main() {
    // Pouzijeme normalu ako farbu (pre vizualizaciu)
    FragColor = aNormal * 0.5 + 0.5; // Normalizo from [-1,1] to [0,1]

    TexCoord = aTexCoord;

    gl_Position = MVP * vec4(aPosition, 1.0);
}