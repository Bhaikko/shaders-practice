#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main()
{
    Normal = normalize(NormalMatrix * VertexNormal);
    Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));

    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition, 1.0);
}