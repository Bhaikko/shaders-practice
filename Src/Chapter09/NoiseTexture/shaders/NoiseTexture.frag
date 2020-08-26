#version 450

#define PI 3.14159265

layout (binding = 0) uniform sampler2D NoiseTex;

uniform vec4 SkyColor = vec4(0.3, 0.3, 0.9, 1.0);
uniform vec4 CloudColor = vec4(1.0, 1.0, 1.0, 1.0);

in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

vec4 PerlinNoise()
{
    vec4 noise = texture(NoiseTex, TexCoord);
    return vec4(noise.aaa, 1.0);
}

vec4 CloudNoise()
{
    vec4 noise = texture(NoiseTex, TexCoord);
    float t = (cos(noise.a * PI) + 1.0) / 2.0;
    vec4 color = mix(SkyColor, CloudColor, t);

    return color;
}

void main()
{
    // FragColor = PerlinNoise();
    FragColor = CloudNoise();
}