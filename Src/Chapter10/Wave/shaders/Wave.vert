#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec4 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform float Time;
uniform float Freq = 2.5;
uniform float Velocity = 2.5;
uniform float Amp = 0.6;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main()
{
    vec4 pos = vec4(VertexPosition, 1.0);

    float u = Freq * pos.x - Velocity * Time;
    pos.y = Amp * sin(u);

    vec3 n = vec3(0.0);
    n.xy = normalize(vec2(cos(u), 1.0));

    Position = ViewMatrix * ModelMatrix * pos;
    Normal = NormalMatrix * VertexNormal;
    TexCoord = VertexTexCoord;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * pos;
}
