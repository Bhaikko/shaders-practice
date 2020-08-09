#version 450

layout (location = 0) in vec3 VertexPosition;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    gl_Position = ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}