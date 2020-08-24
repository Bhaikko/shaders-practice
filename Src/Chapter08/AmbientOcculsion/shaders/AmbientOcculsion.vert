#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main()
{
    Position = vec3(ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0));
    Normal = NormalMatrix * VertexNormal;
    TexCoord = VertexTexCoord;

    mat2 rotate = mat2(
        vec2(0, 1),
        vec2(-1, 0)
    );
    TexCoord = rotate * TexCoord;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}