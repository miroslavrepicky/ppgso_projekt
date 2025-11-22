#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// Output to fragment shader
out vec3 FragPos;        // Fragment position in world space
out vec3 Normal;         // Normal in world space
out vec2 TexCoord;       // Texture coordinates

// Uniforms
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;  // transpose(inverse(ModelMatrix))

void main() {
    // Position in world space
    FragPos = vec3(ModelMatrix * vec4(aPosition, 1.0));

    // Normal in world space (use NormalMatrix for correct transformation)
    Normal = normalize(NormalMatrix * aNormal);

    // Texture coordinates (passthrough)
    TexCoord = aTexCoord;

    // Final position (clip space)
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}