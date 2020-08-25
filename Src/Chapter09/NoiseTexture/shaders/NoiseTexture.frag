#version 450

layout (binding = 0) uniform sampler2D NoiseTex;

in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec4 noise = texture(NoiseTex, TexCoord);
    FragColor = vec4(noise.aaa, 1.0);

    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}