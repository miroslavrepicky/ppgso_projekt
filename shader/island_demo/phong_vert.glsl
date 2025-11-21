#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// Output to fragment shader
out vec3 FragPos;        // Pozicia fragmentu v world space
out vec3 Normal;         // Normala v world space
out vec2 TexCoord;       // Texturove koordinaty

// Uniforms
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;  // transpose(inverse(ModelMatrix))

void main() {
    // Pozicia vo world space
    FragPos = vec3(ModelMatrix * vec4(aPosition, 1.0));

    // Normala vo world space (pouzijeme NormalMatrix pre spravnu transformaciu)
    Normal = normalize(NormalMatrix * aNormal);

    // Texturove koordinaty (passthrough)
    TexCoord = aTexCoord;

    // Finalna pozicia (clip space)
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}