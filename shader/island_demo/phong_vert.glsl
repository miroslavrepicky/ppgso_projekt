#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main() {
    FragPos = vec3(ModelMatrix * vec4(aPosition, 1.0));
    Normal = normalize(NormalMatrix * aNormal);
    TexCoord = aTexCoord;

    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}