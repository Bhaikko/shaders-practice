#version 450

const float PI = 3.14159265359;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexAge;

uniform int Pass;

layout (xfb_buffer = 0, xfb_offset = 0) out vec3 Position;
layout (xfb_buffer = 1, xfb_offset = 0) out vec3 Velocity;
layout (xfb_buffer = 2, xfb_offset = 0) out float Age;

out float Transparency;
out vec2 TexCoord;

uniform float Time;
uniform float DeltaTime;
uniform vec3 Acceleration;
uniform float ParticleLifetime;
uniform float ParticleSize;
uniform mat3 EmitterBasis;
uniform vec3 EmitterPosition;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

layout (binding = 1) uniform sampler1D RandomTex;

const vec3 offsets[] = vec3[](
    vec3(-0.5, -0.5, 0.0),
    vec3( 0.5, -0.5, 0.0),
    vec3( 0.5,  0.5, 0.0),
    vec3(-0.5, -0.5, 0.0),
    vec3( 0.5,  0.5, 0.0),
    vec3(-0.5,  0.5, 0.0)
);

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
    float velocity = mix(0.1, 0.5, texelFetch(RandomTex, 2 * gl_VertexID, 0).r);
    return EmitterBasis * vec3(0, velocity, 0);
}

vec3 RandomInitialPosition()
{
    float offset = mix(-2.0, 2.0, texelFetch(RandomTex, 2 * gl_VertexID + 1, 0).r);
    return EmitterBasis * vec3(offset, 0, 0);
}

void Update()
{
    Age = VertexAge + DeltaTime;

    if (VertexAge < 0 || VertexAge > ParticleLifetime) {
        Position = RandomInitialPosition();
        Velocity = RandomInitialVelocity();
        if (VertexAge > ParticleLifetime) {
            Age = (VertexAge - ParticleLifetime) + DeltaTime;
        }
    } else {
        Position = VertexPosition + VertexVelocity * DeltaTime;
        Velocity = VertexVelocity + Acceleration * DeltaTime;
    }
}

void Render()
{
    Transparency = 0.0;
    vec3 posCam = vec3(0.0);

    if (VertexAge >= 0.0) {
        posCam = (ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0)).xyz + offsets[gl_VertexID] * ParticleSize;
        if (VertexAge < 0) {
            Transparency = 0.0f;
        } else {
            Transparency = clamp(1.0 - VertexAge / ParticleLifetime, 0, 1);
        }
    }
    TexCoord = texCoords[gl_VertexID];

    gl_Position = ProjectionMatrix * vec4(posCam, 1.0);
}


void main() 
{
    if (Pass == 1) {
        Update();
    } else {
        Render();
    }
}
