#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 ReflectDir;

uniform sampler2D texture_diffuse1;
uniform samplerCube skybox;
uniform bool useTexture;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 textureColor = texture(texture_diffuse1, TexCoords).rgb;

    vec3 envColor = texture(skybox, ReflectDir).rgb;

    vec3 resultColor = mix(textureColor, envColor, 0.5);
    FragColor = vec4(resultColor, 1.0);
}
