#version 450


centroid in vec2 TexCoord;
// in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 yellow = vec3(1.0,1.0,0.0);
    vec3 color = vec3(0.0);
    if( TexCoord.s >= 0.7 ) color = yellow;
    FragColor = vec4( color , 1.0 );
}