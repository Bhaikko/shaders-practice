#pragma once

#include "./../../../Helper/common_headers.h"

class Cloth
{
private:
    Shader renderShader, computeShader, computeNormalShader;
    Texture clothTexture;

    GLuint clothVAO;
    GLuint numElements;

    glm::ivec2 nParticles;
    glm::vec2 clothSize;

    float time, deltaTime, speed;

    GLuint readBuffer;
    GLuint positionBuffers[2], velocityBuffer[2];
    GLuint normalBuffer, elementBuffer, textureBuffer;

public:
    Cloth();
    
    void Init();
    void Update(float deltaTime);
    void Render(glm::mat4& view, glm::mat4& projection);

private:
    void InitBuffers();
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

};