#version 330 core

// Light types
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

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
uniform vec3 viewPos;
uniform Material material;

// Light 0 uniforms
uniform int light0_type;
uniform int light0_enabled;
uniform vec3 light0_ambient;
uniform vec3 light0_diffuse;
uniform vec3 light0_specular;
uniform vec3 light0_direction;
uniform vec3 light0_position;
uniform float light0_constantAttenuation;
uniform float light0_linearAttenuation;
uniform float light0_quadraticAttenuation;
uniform float light0_innerCutoff;
uniform float light0_outerCutoff;

// Light 1 uniforms
uniform int light1_type;
uniform int light1_enabled;
uniform vec3 light1_ambient;
uniform vec3 light1_diffuse;
uniform vec3 light1_specular;
uniform vec3 light1_direction;
uniform vec3 light1_position;
uniform float light1_constantAttenuation;
uniform float light1_linearAttenuation;
uniform float light1_quadraticAttenuation;
uniform float light1_innerCutoff;
uniform float light1_outerCutoff;

// Light 2 uniforms
uniform int light2_type;
uniform int light2_enabled;
uniform vec3 light2_ambient;
uniform vec3 light2_diffuse;
uniform vec3 light2_specular;
uniform vec3 light2_direction;
uniform vec3 light2_position;
uniform float light2_constantAttenuation;
uniform float light2_linearAttenuation;
uniform float light2_quadraticAttenuation;
uniform float light2_innerCutoff;
uniform float light2_outerCutoff;

uniform int numLights;
uniform sampler2D Texture;
uniform bool useTexture;
uniform bool useBlinnPhong;

// Forward declarations
vec3 CalcDirectionalLight(vec3, vec3, vec3, vec3, vec3, vec3);
vec3 CalcPointLight(vec3, vec3, vec3, vec3, float, float, float, vec3, vec3, vec3);
vec3 CalcSpotLight(vec3, vec3, vec3, vec3, vec3, float, float, float, float, float, vec3, vec3, vec3);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    // DEBUG: Manualne vypocitaj Light 0 bez funkcie
    if (numLights > 0 && light0_enabled == 1) {
        vec3 lightDir = normalize(-light0_direction);

        // Ambient
        vec3 ambient = light0_ambient * material.ambient;

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light0_diffuse * (diff * material.diffuse);

        result = ambient + diffuse;
    }

    FragColor = vec4(result, 1.0);
    return;

    // Light 0
    if (numLights > 0 && light0_enabled == 1) {
        if (light0_type == LIGHT_DIRECTIONAL) {
            result += CalcDirectionalLight(light0_ambient, light0_diffuse, light0_specular, light0_direction, norm, viewDir);
        }
        else if (light0_type == LIGHT_POINT) {
            result += CalcPointLight(light0_ambient, light0_diffuse, light0_specular, light0_position, light0_constantAttenuation, light0_linearAttenuation, light0_quadraticAttenuation, norm, FragPos, viewDir);
        }
        else if (light0_type == LIGHT_SPOT) {
            result += CalcSpotLight(light0_ambient, light0_diffuse, light0_specular, light0_position, light0_direction, light0_constantAttenuation, light0_linearAttenuation, light0_quadraticAttenuation, light0_innerCutoff, light0_outerCutoff, norm, FragPos, viewDir);
        }
    }

    // Light 1
    if (numLights > 1 && light1_enabled == 1) {
        if (light1_type == LIGHT_DIRECTIONAL) {
            result += CalcDirectionalLight(light1_ambient, light1_diffuse, light1_specular, light1_direction, norm, viewDir);
        }
        else if (light1_type == LIGHT_POINT) {
            result += CalcPointLight(light1_ambient, light1_diffuse, light1_specular, light1_position, light1_constantAttenuation, light1_linearAttenuation, light1_quadraticAttenuation, norm, FragPos, viewDir);
        }
        else if (light1_type == LIGHT_SPOT) {
            result += CalcSpotLight(light1_ambient, light1_diffuse, light1_specular, light1_position, light1_direction, light1_constantAttenuation, light1_linearAttenuation, light1_quadraticAttenuation, light1_innerCutoff, light1_outerCutoff, norm, FragPos, viewDir);
        }
    }

    // Light 2
    if (numLights > 2 && light2_enabled == 1) {
        if (light2_type == LIGHT_DIRECTIONAL) {
            result += CalcDirectionalLight(light2_ambient, light2_diffuse, light2_specular, light2_direction, norm, viewDir);
        }
        else if (light2_type == LIGHT_POINT) {
            result += CalcPointLight(light2_ambient, light2_diffuse, light2_specular, light2_position, light2_constantAttenuation, light2_linearAttenuation, light2_quadraticAttenuation, norm, FragPos, viewDir);
        }
        else if (light2_type == LIGHT_SPOT) {
            result += CalcSpotLight(light2_ambient, light2_diffuse, light2_specular, light2_position, light2_direction, light2_constantAttenuation, light2_linearAttenuation, light2_quadraticAttenuation, light2_innerCutoff, light2_outerCutoff, norm, FragPos, viewDir);
        }
    }

    if (useTexture) {
        vec4 texColor = texture(Texture, TexCoord);
        result *= texColor.rgb;
    }

    FragColor = vec4(result, 1.0);
}

// Directional Light
vec3 CalcDirectionalLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 direction, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-direction);

    vec3 ambientResult = ambient * material.ambient;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseResult = diffuse * (diff * material.diffuse);

    vec3 specularResult = vec3(0.0);
    if (diff > 0.0) {
        if (useBlinnPhong) {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            specularResult = specular * (spec * material.specular);
        } else {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specularResult = specular * (spec * material.specular);
        }
    }

    return ambientResult + diffuseResult + specularResult;
}

// Point Light
vec3 CalcPointLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, float constAtt, float linAtt, float quadAtt, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(position - fragPos);

    vec3 ambientResult = ambient * material.ambient;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseResult = diffuse * (diff * material.diffuse);

    vec3 specularResult = vec3(0.0);
    if (diff > 0.0) {
        if (useBlinnPhong) {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            specularResult = specular * (spec * material.specular);
        } else {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specularResult = specular * (spec * material.specular);
        }
    }

    float distance = length(position - fragPos);
    float attenuation = 1.0 / (constAtt + linAtt * distance + quadAtt * distance * distance);

    ambientResult *= attenuation;
    diffuseResult *= attenuation;
    specularResult *= attenuation;

    return ambientResult + diffuseResult + specularResult;
}

// Spot Light
vec3 CalcSpotLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, vec3 direction, float constAtt, float linAtt, float quadAtt, float innerCut, float outerCut, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(position - fragPos);

    vec3 ambientResult = ambient * material.ambient;

    float theta = dot(lightDir, normalize(-direction));
    float epsilon = innerCut - outerCut;
    float intensity = clamp((theta - outerCut) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseResult = diffuse * (diff * material.diffuse);

    vec3 specularResult = vec3(0.0);
    if (diff > 0.0) {
        if (useBlinnPhong) {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
            specularResult = specular * (spec * material.specular);
        } else {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specularResult = specular * (spec * material.specular);
        }
    }

    float distance = length(position - fragPos);
    float attenuation = 1.0 / (constAtt + linAtt * distance + quadAtt * distance * distance);

    diffuseResult *= intensity * attenuation;
    specularResult *= intensity * attenuation;
    ambientResult *= attenuation;

    return ambientResult + diffuseResult + specularResult;
}