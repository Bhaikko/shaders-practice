#version 450

layout (binding = 0) uniform sampler2D ParticleTex;

in float Transparency;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, TexCoord);
    FragColor.a *= Transparency;
}