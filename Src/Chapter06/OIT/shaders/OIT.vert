#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

out vec3 Position;
out vec3 Normal;


void main()
{
    Normal = normalize(NormalMatrix * VertexNormal);
    Position = vec3(ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0));

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}
