#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

uniform vec3 WorldCameraPosition;

out vec3 ReflectDir;
out vec3 RefractDir;
// out vec3 

void main()
{
    vec3 worldPos = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
    vec3 worldNorm = vec3(NormalMatrix * VertexNormal);
    vec3 worldView = normalize(worldPos - WorldCameraPosition);

    RefractDir = refract(-worldView, worldNorm, 1.517);
    ReflectDir = reflect(-worldView, worldNorm);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}