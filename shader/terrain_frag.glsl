#version 330 core

in vec3 vNormal;
in vec3 vWorldPos;
in vec2 vUV;

out vec4 FragColor;

void main() {
    vec3 baseColor = mix(vec3(0.2, 0.5, 0.2), vec3(0.8, 0.8, 0.5), vUV.y);
    vec3 lightDir = normalize(vec3(0.3, 1.0, 0.5));
    float diff = max(dot(normalize(vNormal), lightDir), 0.0);
    vec3 color = baseColor * (0.4 + 0.6*diff);
    FragColor = vec4(color, 1.0);
}