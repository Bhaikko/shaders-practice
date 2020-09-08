#pragma once

#include "./../../../Helper/common_headers.h"
#include "./../ParticlesUtils.h"

class Fire
{
private:
    Shader shader;
    Texture fire;

    GLuint positionBuffer[2], velocityBuffer[2], ageBuffer[2];
    GLuint particleArray[2];
    GLuint feedback[2];
    GLuint drawBuffer;

    int nParticles;
    float time, deltaTime;
    float particleSize, particleLifeTime;


public:
    Fire();

    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);
    void Update(float deltaTime);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void InitBuffers();

};