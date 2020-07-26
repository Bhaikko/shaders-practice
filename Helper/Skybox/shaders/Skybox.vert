#version 450

layout (location = 0) in vec3 pos;

out vec3 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    TexCoord = pos;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(pos, 1.0);
}