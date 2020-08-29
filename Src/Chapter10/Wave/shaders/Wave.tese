#version 450

layout (quads, equal_spacing, ccw) in;

in vec3 TENormal[];
in vec2 TETexCoord[];

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

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p10 = gl_in[1].gl_Position;
    vec4 p11 = gl_in[2].gl_Position;
    vec4 p01 = gl_in[3].gl_Position;

    // Linear Interpolation
    vec4 pos = 
        p00 * (1.0 - u) * (1.0 - v) + 
        p10 * u * (1.0 - v) + 
        p01 * v * (1.0 - u) + 
        p11 * u * v;


    float theta = Freq * pos.x - Velocity * Time;
    pos.y = Amp * sin(theta);

    vec3 n = vec3(0.0);
    n.xy = normalize(vec2(cos(u), 1.0));

    Position = ViewMatrix * ModelMatrix * pos;
    Normal = NormalMatrix * n;
    TexCoord = vec2(0.0, 0.0);

    // Transform to clio coordinates
    gl_Position = ProjectionMatrix * Position;
}