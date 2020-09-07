#pragma once

#include "./../../../Helper/common_headers.h"
#include "./../ParticlesUtils.h"

class Instanced
{
private:
    Shader shader;
    Model torus;

    GLuint positionBuffer[2], velocityBuffer[2], rotationBuffer[2], ageBuffer[2];
    GLuint particleArray[2];
    GLuint feedback[2];

    int nParticles;
    float particleLifetime;
    float time, deltaTime;
    int drawBuffer;

    glm::vec3 emitterPosition, emitterDirection;

public:
    Instanced();

    void Init();
    void Update(float deltaTime);
    void Render(glm::mat4& view, glm::mat4& projection);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void InitBuffers();
};