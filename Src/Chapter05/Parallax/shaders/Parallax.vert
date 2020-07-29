#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec4 VertexTangent;

uniform struct LightInfo {
    vec4 Position;
    vec3 L;
    vec3 La;
} Light;

out vec3 LightDir;
out vec2 TexCoord;
out vec3 ViewDir;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main()
{
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec3 tangent = normalize(NormalMatrix * VertexTangent.xyz);

    vec3 binormal = normalize(cross(norm, tangent));

    mat3 toObjectLocal = transpose(mat3(tangent, binormal, norm));

    // Transform light direction and view direction to tangent space
    vec3 pos = vec3(ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0));

    LightDir = normalize(toObjectLocal * (Light.Position.xyz - pos));
    ViewDir = toObjectLocal * normalize(-pos);
    TexCoord = VertexTexCoord;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}