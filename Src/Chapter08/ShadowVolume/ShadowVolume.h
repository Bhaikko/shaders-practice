#pragma once

#include "./../../../Helper/common_headers.h"
#include "./../../../Helper/Frustum/Frustum.h"

class ShadowVolume
{
private:
    Shader volumeShader, renderShader, compShader;
    GLuint colorDepthFBO;

    Texture brickTexture, mossTexture;

    Model screenQuad, plane, bunny;

    glm::vec4 lightPos;
    float angle, rotSpeed;

    GLint width, height;


public:
    ShadowVolume(GLint width, GLint height);
    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);
    void Update(float deltaTime);

private:
    void SetMatrices(Shader& shader, glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void SetupFBO();
    void DrawScene(Shader& shader, glm::mat4& view, glm::mat4& projection, bool onlyShadowCasters);
    void Pass1(glm::mat4& view, glm::mat4& projection);
    void Pass2(glm::mat4& view, glm::mat4& projection);
    void Pass3();
    void UpdateLight();
};