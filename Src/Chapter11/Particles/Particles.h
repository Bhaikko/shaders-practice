#pragma once

#include "./../../../Helper/common_headers.h"

class Particle
{
private:
    Shader renderShader, computeShader;

    glm::ivec3 nParticles;
    GLuint totalParticles;

    float time, deltaTime, angle, speed;

    GLuint particlesVAO;
    GLuint bhVAO, bhBuffer; // Black hole VAO and buffer

    glm::vec4 bh1, bh2;

public:
    Particle();
    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);
    void Update(float deltaTime);

private:
    void InitBuffers();
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
};

/* 
    The number of invocations of a compute shader is governed by the user-defined compute
    space. This space is divided into a number of work groups. Each work group is then broken
    down into a number of invocations. We think of this in terms of the global compute space (all
    shader invocations) and the local work group space (the invocations within a particular work
    group). The compute space can be defined as a 1, 2 or 3 dimensional space
*/

// OpenGL provides atomic operations and memory barriers that can help with 
// the communication between invocations