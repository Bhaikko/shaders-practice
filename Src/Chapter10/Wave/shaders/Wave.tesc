#version 450

layout (vertices = 4) out;

uniform int TessLevel = 8;

in vec3 TCNormal[];
in vec2 TCTexCoord[];

out vec3 TENormal[];
out vec2 TETexCoord[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = float(TessLevel);
    gl_TessLevelOuter[1] = float(TessLevel);
    gl_TessLevelOuter[2] = float(TessLevel);
    gl_TessLevelOuter[3] = float(TessLevel);

    gl_TessLevelInner[0] = float(TessLevel); 
    gl_TessLevelInner[1] = float(TessLevel); 

    TENormal[gl_InvocationID] = TCNormal[gl_InvocationID];
    TETexCoord[gl_InvocationID] = TCTexCoord[gl_InvocationID];
}