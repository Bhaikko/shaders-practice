#version 450

#define PI 3.14159265

layout (binding = 0) uniform sampler2D NoiseTex;

uniform vec4 SkyColor = vec4(0.3, 0.3, 0.9, 1.0);
uniform vec4 CloudColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform vec4 DarkWoodColor = vec4(0.8, 0.5, 0.1, 1.0);
uniform vec4 LightWoodColor = vec4(1.0, 0.75, 0.25, 1.0);

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

vec4 WoodNoise()
{
    // Transforming texture Coordinates to define 
    // "Slice" of log
    vec2 tc = TexCoord;
    if (tc.s > 0.5) {
        tc.s = 1.0 - tc.s;
    }

    vec4 cyl = vec4(tc, 0.0, 1.0);

    // Distance from log's y axis
    float dist = length(cyl.xz);


    vec4 noise = texture(NoiseTex, tc);
    dist += noise.b * 2.5;

    // Determin the color as mixture of
    // Light and Dark wood Color
    float t = 1.0 - abs(fract(dist) * 2.0 - 1.0);
    t = smoothstep(0.1, 0.2, t);
    vec4 color = mix(DarkWoodColor, LightWoodColor, t);

    return color;
}

void main()
{
    // FragColor = PerlinNoise();
    // FragColor = CloudNoise();
    FragColor = WoodNoise();
}