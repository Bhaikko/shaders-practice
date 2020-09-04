#pragma once

#include "./../../../Helper/common_headers.h"
#include "./../ParticlesUtils.h"

class TransformFeedback
{
private:
    Shader shader;
    Texture texture;
    Random rand;

    glm::vec3 emitterPos, emitterDir;

    // Particle Buffers
    GLuint posBuf[2], velBuf[2], ageBuf[2];

    // Particle VAOs
    GLuint particles[2];

    // Transform Feedbacks
    GLuint feedback[2];

    GLuint drawBuf;
    
    int nParticles;
    float particleLifetime;
    float time, deltaTime;

public:
    TransformFeedback();

    void Init();
    void Update(float time);
    void Render(glm::mat4& view, glm::mat4& projection);

private:
    void InitBuffers();
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

};