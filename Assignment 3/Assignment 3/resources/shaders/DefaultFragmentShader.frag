#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos); 

    vec3 result = texture(texture_diffuse1, TexCoords).rgb;
    FragColor = vec4(result, 1.0);
}
