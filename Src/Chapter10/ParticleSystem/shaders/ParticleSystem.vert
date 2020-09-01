#version 450

layout (location = 0) in vec3 VertexInitVelocity;
layout (location = 1) in float VertexBirthTime;

out float Transparency;
out vec2 TexCoord;

uniform float Time;
uniform vec3 Gravity;
uniform float ParticleLifeTime;
uniform float ParticleSize;
uniform vec3 EmitterPos;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

// Offsets to position in Camera Coordinates for each vertex of particle's quad
const vec3 offsets[] = vec3[](
    vec3(-0.5, -0.5, 0.0), 
    vec3( 0.5, -0.5, 0.0),
    vec3( 0.5,  0.5, 0.0),
    vec3(-0.5, -0.5, 0.0),
    vec3( 0.5,  0.5, 0.0),
    vec3(-0.5,  0.5, 0.0)
);

// Texture Coordinates for each Vertex of particle's quad
const vec2 texCoords[] = vec2[](
    vec2(0, 0),
    vec2(1, 0),
    vec2(1, 1),
    vec2(0, 0),
    vec2(1, 1),
    vec2(0, 1)
);

void main()
{
    vec3 cameraPos;
    float t = Time - VertexBirthTime;

    if (t >= 0 && t < ParticleLifeTime) {
        vec3 pos = EmitterPos + VertexInitVelocity * t + Gravity * t * t;
        cameraPos = (ViewMatrix * ModelMatrix * vec4(pos, 1)).xyz + (offsets[gl_VertexID] * ParticleSize);
        Transparency = mix(1, 0, t / ParticleLifeTime);
    } else {
        cameraPos = vec3(0);
        Transparency = 0.0;
    }

    // Contains the index of the current vertex
    TexCoord = texCoords[gl_VertexID];

    gl_Position = ProjectionMatrix * vec4(cameraPos, 1.0);
}
