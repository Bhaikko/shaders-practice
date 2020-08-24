#version 450

uniform struct LightInfo {
    vec4 Position;
    vec3 Intensity;
} Light;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D AOTex;
layout (binding = 1) uniform sampler2D DiffTex;


layout (location = 0) out vec4 FragColor;


vec3 PhongModelDiffuse()
{
    vec3 n = Normal;
    vec3 s = normalize(vec3(Light.Position) - Position);
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffColor = texture(DiffTex, TexCoord).rgb;
    return Light.Intensity * diffColor * sDotN;
}

void main()
{
    vec3 diffuse = PhongModelDiffuse();

    vec4 aoFactor = texture(AOTex, TexCoord);

    FragColor = vec4(diffuse * aoFactor.r, 1.0);
}