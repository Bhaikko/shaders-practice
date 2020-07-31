#pragma once 

#include "./../../../Helper/common_headers.h"

class Blur
{
    private:
        Shader shader;
        Model screenQuad, teapot, torus, cube;

        GLuint renderFBO, renderTex;        // For pass 3
        GLuint intermediateFBO, intermediateTex; // For Pass 2

        GLint width, height;
    public:
        Blur(GLint width, GLint height);
        void Init();
        void Render(glm::mat4 view, glm::mat4 projection);

    private:

        void SetupFBO();
        void Pass1(glm::mat4 view, glm::mat4 projection);
        void Pass2();
        void Pass3();
        float Guass(float x, float sigma2);
        void SetupMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
};