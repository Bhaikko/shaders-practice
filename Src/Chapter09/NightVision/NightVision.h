#pragma once

#include "./../../../Helper/common_headers.h"

class NightVision
{

private:
    Shader shader;
    Model screenQuad;
    Model plane, torus, teapot;

    GLuint pass1Index, pass2Index;
    GLuint renderFBO, renderTex;
    GLuint noiseTex;

    GLint width, height;

public:
    NightVision(GLint width, GLint height);
    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void SetupFBO();
    void Pass1(glm::mat4& view, glm::mat4& projection);
    void Pass2();
};