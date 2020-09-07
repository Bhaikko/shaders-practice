#version 450

const float PI = 3.14159265359;

uniform int Pass;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCorrd;
layout (location = 2) in vec3 VertexNormal;

layout (location = 3) in vec3 ParticlePosition;
layout (location = 4) in vec3 ParticleVelocity;
layout (location = 5) in float ParticleAge;
layout (location = 6) in vec2 ParticleRotation;

// To transform feedback
layout (xfb_buffer = 0, xfb_offset = 0) out vec3 Position;
layout (xfb_buffer = 1, xfb_offset = 0) out vec3 Velocity;
layout (xfb_buffer = 2, xfb_offset = 0) out float Age;
layout (xfb_buffer = 3, xfb_offset = 0) out vec2 Rotation;

// To fragment shader
out vec3 fPosition;
out vec3 fNormal;

uniform float Time;
uniform float DeltaTime;
uniform vec3 Acceleration;
uniform float ParticleLifetime;
uniform vec3 EmitterPosition;
uniform mat3 EmitterBasis;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

layout (binding = 0) uniform sampler1D RandomTex;

vec3 RandomInitialVelocity()
{
    float theta = mix(0.0, PI / 6.0, texelFetch(RandomTex, 4 * gl_VertexID, 0).r);
    float phi = mix(0.0, 2.0 * PI, texelFetch(RandomTex, 4 * gl_VertexID + 1, 0).r);
    float velocity = mix(1.25, 1.5, texelFetch(RandomTex, 4 * gl_VertexID + 2, 0).r);

    vec3 v = vec3(
        sin(theta) * cos(phi),
        cos(theta),
        sin(theta) * sin(phi)
    );

    return normalize(EmitterBasis * v) * velocity;
}

float RandomInitialRotationalVelocity() {
    return mix(-15.0, 15.0, texelFetch(RandomTex, 4 * gl_VertexID + 3, 0).r);
}

void Update()
{
    if (ParticleAge < 0 || ParticleAge > ParticleLifetime) {
        Position = EmitterPosition;
        Velocity = RandomInitialVelocity();
        Rotation = vec2(0.0, RandomInitialRotationalVelocity());

        if (ParticleAge < 0) {
            Age = ParticleAge + DeltaTime;
        } else {
            Age = (ParticleAge - ParticleLifetime) + DeltaTime;
        }
    } else {
        // Updating using Euler Method
        Position = ParticlePosition + ParticleVelocity * DeltaTime + gl_VertexID;
        Velocity = ParticleVelocity + Acceleration * DeltaTime;

        Rotation.x = mod(ParticleRotation.x + ParticleRotation.y * DeltaTime, 2.0 * PI);
        Rotation.y = ParticleRotation.y;
        Age = ParticleAge + DeltaTime;
    }
}

void Render()
{
    float cs = cos(ParticleRotation.x);
    float sn = sin(ParticleRotation.x);

    mat4 RotationAndTranslation = mat4(
        1, 0, 0, 0,
        0, cs, sn, 0,
        0, -sn, cs, 0,
        ParticlePosition.x, ParticlePosition.y, ParticlePosition.z, 1
    );

    mat4 m = ViewMatrix * ModelMatrix * RotationAndTranslation;
    fPosition = (m * vec4(VertexPosition, 1.0)).xyz;
    fNormal = NormalMatrix * VertexNormal;

    gl_Position = ProjectionMatrix * vec4(fPosition, 1.0);
}

void main()
{
    if (Pass == 1) {
        Update();
    } else {
        Render();
    }
}