#version 450

layout (location = 0) in vec3 VertexPosition;

uniform mat4 MV;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * MV * vec4(VertexPosition, 1.0);
}