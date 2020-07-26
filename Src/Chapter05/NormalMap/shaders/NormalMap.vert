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
    vec3 tang = normalize(NormalMatrix * vec3(VertexTangent));

    vec3 binormal = normalize(cross(norm, tang)) * VertexTangent.w;

    // Mat3 constructor creates matrix as column major
    mat3 toObjectLocal = mat3(
        tang.x, binormal.x, norm.x,
        tang.y, binormal.y, norm.y,
        tang.z, binormal.z, norm.z 
    );

    // Transform light direction and view direction to tangent space
    vec3 pos = vec3(ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0));
    LightDir = toObjectLocal * (Light.Position.xyz - pos);

    ViewDir = toObjectLocal * normalize(-pos);
    TexCoord = VertexTexCoord;

    // Texcoords fix
    mat2 rotate = mat2(
        vec2(0, 1),
        vec2(-1, 0)
    );
    TexCoord = rotate * TexCoord;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}