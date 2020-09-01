#pragma once

#include "./../../../Helper/common_headers.h"
#include "./../ParticlesUtils.h"

class ParticleSystem
{
private:
    Random rand;
    Shader shader;
    Texture bluewWater;
    GLuint initVelocity, startTime, particles, nParticles;

    glm::vec3 emitterPos, emitterDir;

    float time, particleLifetime;

public:
    ParticleSystem();

    void Init();
    void Update(float deltaTime);
    void Render(glm::mat4& view, glm::mat4& projection);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void InitBuffers();
    float RandFloat();


};