#version 330 core

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

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

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    // Light 0
    if (numLights > 0 && light0_enabled == 1) {
        vec3 lightResult = vec3(0.0);

        if (light0_type == LIGHT_DIRECTIONAL) {
            vec3 lightDir = normalize(-light0_direction);
            vec3 ambient = light0_ambient * material.ambient;
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light0_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light0_specular * (spec * material.specular);
            }
            lightResult = ambient + diffuse + specular;
        }
        else if (light0_type == LIGHT_POINT) {
            vec3 lightDir = normalize(light0_position - FragPos);
            vec3 ambient = light0_ambient * material.ambient;
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light0_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light0_specular * (spec * material.specular);
            }
            float distance = length(light0_position - FragPos);
            float attenuation = 1.0 / (light0_constantAttenuation + light0_linearAttenuation * distance + light0_quadraticAttenuation * distance * distance);
            lightResult = (ambient + diffuse + specular) * attenuation;
        }
        else if (light0_type == LIGHT_SPOT) {
            vec3 lightDir = normalize(light0_position - FragPos);
            vec3 ambient = light0_ambient * material.ambient;
            float theta = dot(lightDir, normalize(-light0_direction));
            float epsilon = light0_innerCutoff - light0_outerCutoff;
            float intensity = clamp((theta - light0_outerCutoff) / epsilon, 0.0, 1.0);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light0_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light0_specular * (spec * material.specular);
            }
            float distance = length(light0_position - FragPos);
            float attenuation = 1.0 / (light0_constantAttenuation + light0_linearAttenuation * distance + light0_quadraticAttenuation * distance * distance);
            lightResult = (ambient + (diffuse + specular) * intensity) * attenuation;
        }

        result += lightResult;
    }

    // Light 1
    if (numLights > 1 && light1_enabled == 1) {
        vec3 lightResult = vec3(0.0);

        if (light1_type == LIGHT_DIRECTIONAL) {
            vec3 lightDir = normalize(-light1_direction);
            vec3 ambient = light1_ambient * material.ambient;
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light1_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light1_specular * (spec * material.specular);
            }
            lightResult = ambient + diffuse + specular;
        }
        else if (light1_type == LIGHT_POINT) {
            vec3 lightDir = normalize(light1_position - FragPos);
            vec3 ambient = light1_ambient * material.ambient;
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light1_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light1_specular * (spec * material.specular);
            }
            float distance = length(light1_position - FragPos);
            float attenuation = 1.0 / (light1_constantAttenuation + light1_linearAttenuation * distance + light1_quadraticAttenuation * distance * distance);
            lightResult = (ambient + diffuse + specular) * attenuation;
        }
        else if (light1_type == LIGHT_SPOT) {
            vec3 lightDir = normalize(light1_position - FragPos);
            vec3 ambient = light1_ambient * material.ambient;
            float theta = dot(lightDir, normalize(-light1_direction));
            float epsilon = light1_innerCutoff - light1_outerCutoff;
            float intensity = clamp((theta - light1_outerCutoff) / epsilon, 0.0, 1.0);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light1_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light1_specular * (spec * material.specular);
            }
            float distance = length(light1_position - FragPos);
            float attenuation = 1.0 / (light1_constantAttenuation + light1_linearAttenuation * distance + light1_quadraticAttenuation * distance * distance);
            lightResult = (ambient + (diffuse + specular) * intensity) * attenuation;
        }

        result += lightResult;
    }

    // Light 2
    if (numLights > 2 && light2_enabled == 1) {
        vec3 lightResult = vec3(0.0);

        if (light2_type == LIGHT_DIRECTIONAL) {
            vec3 lightDir = normalize(-light2_direction);
            vec3 ambient = light2_ambient * material.ambient;
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light2_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light2_specular * (spec * material.specular);
            }
            lightResult = ambient + diffuse + specular;
        }
        else if (light2_type == LIGHT_POINT) {
            vec3 lightDir = normalize(light2_position - FragPos);
            vec3 ambient = light2_ambient * material.ambient;
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light2_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light2_specular * (spec * material.specular);
            }
            float distance = length(light2_position - FragPos);
            float attenuation = 1.0 / (light2_constantAttenuation + light2_linearAttenuation * distance + light2_quadraticAttenuation * distance * distance);
            lightResult = (ambient + diffuse + specular) * attenuation;
        }
        else if (light2_type == LIGHT_SPOT) {
            vec3 lightDir = normalize(light2_position - FragPos);
            vec3 ambient = light2_ambient * material.ambient;
            float theta = dot(lightDir, normalize(-light2_direction));
            float epsilon = light2_innerCutoff - light2_outerCutoff;
            float intensity = clamp((theta - light2_outerCutoff) / epsilon, 0.0, 1.0);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light2_diffuse * (diff * material.diffuse);
            vec3 specular = vec3(0.0);
            if (diff > 0.0) {
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
                specular = light2_specular * (spec * material.specular);
            }
            float distance = length(light2_position - FragPos);
            float attenuation = 1.0 / (light2_constantAttenuation + light2_linearAttenuation * distance + light2_quadraticAttenuation * distance * distance);
            lightResult = (ambient + (diffuse + specular) * intensity) * attenuation;
        }

        result += lightResult;
    }

    if (useTexture) {
        vec4 texColor = texture(Texture, TexCoord);
        result *= texColor.rgb;
    }

    FragColor = vec4(result, 1.0);
}