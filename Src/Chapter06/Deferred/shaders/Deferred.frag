#version 450

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 PositionData;
layout (location = 2) out vec3 NormalData;
layout (location = 3) out vec3 ColorData;

uniform int Pass;

uniform struct LightInfo {
    vec4 Position;
    vec3 L;
} Light;

uniform struct MaterialInfo {
    vec3 Kd;
} Material;

layout (binding = 0) uniform sampler2D PositionTex;
layout (binding = 1) uniform sampler2D NormalTex;
layout (binding = 2) uniform sampler2D ColorTex;

vec3 DiffuseModel(vec3 pos, vec3 norm, vec3 diff)
{
    vec3 s = normalize( vec3(Light.Position) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    return Light.L * diff * sDotN;
}

void Pass1()
{
    // Store Position, normal and diffuse color in textures
    PositionData = Position;
    NormalData = normalize(Normal);
    ColorData = Material.Kd;

}

void Pass2()
{
    // Retrieve position and normal information from textures
    vec3 pos = vec3(texture(PositionTex, TexCoord));
    vec3 norm = vec3(texture(NormalTex, TexCoord));
    vec3 diffColor = vec3(texture(ColorTex, TexCoord));

    FragColor = vec4(DiffuseModel(pos, norm, diffColor), 1.0);
}

void main()
{
    if (Pass == 1) {
        Pass1();
    } else if (Pass == 2) {
        Pass2();
    }
}