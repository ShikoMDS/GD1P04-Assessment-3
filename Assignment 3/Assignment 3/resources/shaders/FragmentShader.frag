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

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.color * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.color * spec * vec3(texture(texture_diffuse1, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.color * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.color * spec * vec3(texture(texture_diffuse1, TexCoords));
    
    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;
    
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = CalculateDirectionalLight(directionalLight, norm, viewDir);
    result += CalculateSpotLight(spotLight, norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}
