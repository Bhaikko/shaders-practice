#version 450

layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 18) out;

in vec3 VPosition[];
in vec3 VNormal[];

uniform vec4 LightPosition;
uniform mat4 ProjectionMatrix;

bool FacesLight(vec3 a, vec3 b, vec3 c)
{
    vec3 n = cross(b - a, c - a);
    vec3 da = LightPosition.xyz - a;
    vec3 db = LightPosition.xyz - b;
    vec3 dc = LightPosition.xyz - c;

    return 
        dot(n, da) > 0 || 
        dot(n, db) > 0 || 
        dot(n, dc) > 0;
}

void EmitEdgeQuad(vec3 a, vec3 b)
{
    gl_Position = ProjectionMatrix * vec4(a, 1);
    EmitVertex();

    gl_Position = ProjectionMatrix * vec4(a - LightPosition.xyz, 0);
    EmitVertex();

    gl_Position = ProjectionMatrix * vec4(b, 1);
    EmitVertex();

    gl_Position = ProjectionMatrix * vec4(b - LightPosition.xyz, 0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    // If main triangle faces the light, check each adjacent triangle
    // If adjacent triangle does not face the light, we output a sihloutte edge quad for the corresponding edge.
    if (FacesLight(VPosition[0], VPosition[2], VPosition[4])) {
        if (!FacesLight(VPosition[0], VPosition[1], VPosition[2])) {
            EmitEdgeQuad(VPosition[0], VPosition[2]);
        }

        if (!FacesLight(VPosition[2], VPosition[3], VPosition[4])) {
            EmitEdgeQuad(VPosition[2], VPosition[4]);
        }

        if (!FacesLight(VPosition[4], VPosition[5], VPosition[0])) {
            EmitEdgeQuad(VPosition[4], VPosition[0]);
        }
    }
}