#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;

out vec2 TexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    TexCoord = VertexTexCoord;
    gl_Position = ModelMatrix * vec4(VertexPosition, 1.0);
}