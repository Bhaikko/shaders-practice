#version 450

// Points are always camera facing
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// Half the width of quad
uniform float Size2;

uniform mat4 ProjectionMatrix;

out vec2 TexCoord;

void main()
{
    gl_Position = ProjectionMatrix * (vec4(-Size2, -Size2, 0.0, 0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = ProjectionMatrix * (vec4(Size2, -Size2, 0.0, 0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = ProjectionMatrix * (vec4(-Size2, Size2, 0.0, 0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = ProjectionMatrix * (vec4(Size2, Size2, 0.0, 0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}