#version 450

layout (location = 0) in vec4 VertexPosition;

out vec3 Position;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main()
{
    Position = (ViewMatrix * ModelMatrix * VertexPosition).xyz;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * VertexPosition;
}