#version 450

in vec3 TexCoord;

layout (location = 0) out vec4 FragColor;

uniform samplerCube Skybox;

void main()
{
    FragColor = texture(Skybox, TexCoord);
}