#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

out vec3 EyeNormal;
out vec4 EyePosition;
out vec4 ProjTexCoord;

void main()
{
    vec4 pos4 = vec4(VertexPosition, 1.0);

    EyeNormal = normalize(NormalMatrix * VertexNormal);
    EyePosition = ViewMatrix * ModelMatrix * pos4;
    ProjTexCoord = ProjectionMatrix * ModelMatrix * pos4;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * pos4;
}