#version 450

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

flat out vec4 axisColor;

void main()
{
    axisColor = vec4(pos, 1.0);
    gl_Position = projection * view * model * vec4(pos, 1.0);
}