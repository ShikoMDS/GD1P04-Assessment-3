#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;

void main()
{
    vec3 color = lightColor;
    FragColor = vec4(color, 1.0);
}
