#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec3 Normal;
out vec3 Position;
out vec4 ShadowCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ShadowMatrix;
uniform mat3 NormalMatrix;

void main()
{
    Position = (ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0)).xyz;
    Normal = normalize(NormalMatrix * VertexNormal);
    ShadowCoord = ShadowMatrix * vec4(VertexPosition, 1.0);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}