#version 450

const float PI = 3.14159265359;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexAge;

uniform int Pass;

// Output to transform feedback buffers 
layout (xfb_buffer = 0, xfb_offset = 0) out vec3 Position;
layout (xfb_buffer = 1, xfb_offset = 0) out vec3 Velocity;
layout (xfb_buffer = 2, xfb_offset = 0) out float Age;

out float Transparency;
out vec2 TexCoord;

uniform float Time;
uniform float DeltaTime;
uniform vec3 Acceleration;
uniform float ParticleLifetime;
uniform vec3 EmitterPosition;
uniform mat3 EmitterBasis;
uniform float ParticleSize;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

layout (binding = 1) uniform sampler1D RandomTex;

// Offsets to Position in camera coordinates for each vertex of particle's quad
const vec3 offsets[] = vec3[](
    vec3(-0.5, -0.5, 0.0),
    vec3( 0.5, -0.5, 0.0),
    vec3( 0.5,  0.5, 0.0),
    vec3(-0.5, -0.5, 0.0),
    vec3( 0.5,  0.5, 0.0),
    vec3(-0.5,  0.5, 0.0)
);

// Texture coordinates for each vertex of the particle's quad
const vec2 texCoords[] = vec2[](
    vec2(0, 0),
    vec2(1, 0),
    vec2(1, 1),
    vec2(0, 0),
    vec2(1, 1),
    vec2(0, 1)
);

vec3 RandomInitialVelocity()
{
    float theta = mix(0.0, PI / 8.0, texelFetch(RandomTex, 3 * gl_VertexID, 0).r);
    float phi = mix(0.0, 2.0 * PI, texelFetch(RandomTex, 3 * gl_VertexID + 1, 0).r);
    float velocity = mix(1.25, 1.5, texelFetch(RandomTex, 3 * gl_VertexID + 2, 0).r);

    vec3 v = vec3(
        sin(theta) * cos(phi),
        cos(theta),
        sin(theta) * sin(phi)
    );

    return normalize(EmitterBasis * v) * velocity;
}

void Update()
{
    if (VertexAge < 0 || VertexAge > ParticleLifetime) {
        // The particle is past it's Lifetime, recycle
        
        Position = EmitterPosition;
        Velocity = RandomInitialVelocity();

        if (VertexAge < 0) {
            Age = VertexAge + DeltaTime;
        } else {
            Age = (VertexAge - ParticleLifetime) + DeltaTime;
        }
    } else {
        // The particle is alive, update
        // Euler's Method
        Position = VertexPosition + VertexVelocity * DeltaTime;
        Velocity = VertexVelocity + Acceleration * DeltaTime;

        // Velocity = VertexVelocity + Acceleration * DeltaTime;
        // Position = VertexPosition + Velocity * DeltaTime;
        Age = VertexAge + DeltaTime;
    }
}

void Render()
{
    Transparency = 0.0;
    vec3 posCam = vec3(0.0);
    if (VertexAge >= 0.0) {
        posCam = (ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0)).xyz + offsets[gl_VertexID] * ParticleSize;
        Transparency = clamp(1.0 - VertexAge / ParticleLifetime, 0.0, 1.0);
    }

    TexCoord = texCoords[gl_VertexID];

    gl_Position = ProjectionMatrix * vec4(posCam, 1.0);
}

void main()
{
    if (Pass == 1) {
        Update();
    } else if (Pass == 2) {
        Render();
    }
}