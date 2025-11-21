#version 330 core

// Maximum number of lights
#define MAX_LIGHTS 10

// Light types
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

// Light structure
struct Light {
    int type;
    int enabled;

// Common properties
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

// Directional & Spot
    vec3 direction;

// Point & Spot
    vec3 position;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

// Spot only
    float innerCutoff;
    float outerCutoff;
};

// Material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Output
out vec4 FragColor;

// Uniforms
uniform vec3 viewPos;           // Camera position
uniform Material material;       // Object material
uniform Light lights[MAX_LIGHTS]; // Light array
uniform int numLights;           // Number of active lights

uniform sampler2D Texture;       // Texture (optional)
uniform bool useTexture;         // Use texture flag

uniform bool useBlinnPhong;      // Blinn-Phong vs classic Phong

// Function prototypes
vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // Normalize normal (moze byt interpolovana)
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Vysledna farba
    vec3 result = vec3(0.0);

    // Vypocitaj prispevok kazdého svetla
    for (int i = 0; i < numLights && i < MAX_LIGHTS; i++) {
        if (lights[i].enabled == 0) continue;

        if (lights[i].type == LIGHT_DIRECTIONAL) {
            result += CalcDirectionalLight(lights[i], norm, viewDir);
        }
        else if (lights[i].type == LIGHT_POINT) {
            result += CalcPointLight(lights[i], norm, FragPos, viewDir);
        }
        else if (lights[i].type == LIGHT_SPOT) {
            result += CalcSpotLight(lights[i], norm, FragPos, viewDir);
        }
    }

    // Ak pouzivame texturu, vynasobime vysledok
    if (useTexture) {
        vec4 texColor = texture(Texture, TexCoord);
        result *= texColor.rgb;
    }

    FragColor = vec4(result, 1.0);
}

// Directional Light
vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular (Blinn-Phong or Phong)
    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        if (useBlinnPhong) {
            // Blinn-Phong: uses half vector
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            specular = light.specular * (spec * material.specular);
        } else {
            // Classic Phong: uses reflection vector
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specular = light.specular * (spec * material.specular);
        }
    }

    return ambient + diffuse + specular;
}

// Point Light
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        if (useBlinnPhong) {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            specular = light.specular * (spec * material.specular);
        } else {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specular = light.specular * (spec * material.specular);
        }
    }

    // Attenuation (distance falloff)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constantAttenuation +
    light.linearAttenuation * distance +
    light.quadraticAttenuation * distance * distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

// Spot Light
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Check if fragment is inside spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        if (useBlinnPhong) {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            specular = light.specular * (spec * material.specular);
        } else {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specular = light.specular * (spec * material.specular);
        }
    }

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constantAttenuation +
    light.linearAttenuation * distance +
    light.quadraticAttenuation * distance * distance);

    // Apply spotlight intensity
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;
    ambient *= attenuation;

    return ambient + diffuse + specular;
}