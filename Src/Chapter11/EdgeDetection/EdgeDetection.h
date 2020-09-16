#pragma once

#include "./../../../Helper/common_headers.h"

class Edge
{
private:
    Shader shader, computeShader;
    Model screenQuad, plane, torus, teapot;
    GLuint pass1Index, pass2Index, fboHandle;

    GLint width, height;

public:
    Edge(GLint width, GLint height);
    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void SetupFBO();
    void Pass1(glm::mat4& view, glm::mat4& projection);
    void Pass2();

};