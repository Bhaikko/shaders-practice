#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;


out vec3 TCNormal;
out vec2 TCTexCoord;


void main()
{
    TCNormal = normalize(VertexNormal);
    TCTexCoord = VertexTexCoord;

    gl_Position = vec4(VertexPosition, 1.0);
}