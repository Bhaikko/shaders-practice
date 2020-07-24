#version 450

in vec3 Position;
in vec3 Normal;

uniform struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 L;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
} Material;

const int levels = 3;
const float scaleFactor = 1.0 / levels;

layout (location = 0) out vec4 FragColor;

vec3 ToonShade()
{
    vec3 n = normalize(Normal);
    vec3 s = normalize(Light.Position.xyz - Position);
    vec3 ambient = Light.La * Material.Ka;

    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = Material.Kd * floor(sDotN * levels) * scaleFactor;

    return ambient + Light.L * diffuse;
}

void main()
{
    FragColor = vec4(ToonShade(), 1.0);
}