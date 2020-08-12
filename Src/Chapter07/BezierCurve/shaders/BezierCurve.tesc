#version 450

// The output patch size does not have to match the input patch size.
layout (vertices = 4) out;

uniform int NumSegments;
uniform int NumStrips;

void main()
{
    // Pass along the vertex position unmodified
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Level 0 is number of strips
    // Level 1 is number of segments
    gl_TessLevelOuter[0] = float(NumStrips);
    gl_TessLevelOuter[1] = float(NumSegments);
}

/* 
    patch out float gl_TessLevelOuter[4];
    patch out float gl_TessLevelInner[2];
    These define the outer and inner tessellation levels used by the tessellation primitive generator. 
    They define how much tessellation to apply to the patch

*/  