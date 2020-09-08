#version 450

layout (binding = 0) uniform sampler2D ParticleTex;

in float Transparency;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, TexCoord);

    // Mix with black as it gets older, to simulate a bit of smoke
    FragColor = vec4(mix(vec3(0, 0, 0), FragColor.xyz, Transparency), FragColor.a);
    FragColor.a *= Transparency;
}