#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 color;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

uniform sampler2D texture_diffuse1;
uniform Material material;
uniform PointLight pointLights[2];
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform vec3 viewPos;

void main()
{
    vec3 ambient = directionalLight.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 norm = normalize(Normal);
    vec3 result = ambient;

    vec3 lightDir = normalize(-directionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.color * diff * vec3(texture(texture_diffuse1, TexCoords));
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLight.color * spec * vec3(texture(texture_diffuse1, TexCoords));
    
    result += diffuse + specular;

    FragColor = vec4(result, 1.0);
}
