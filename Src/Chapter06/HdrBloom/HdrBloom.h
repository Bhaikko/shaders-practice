#pragma once 

#include "./../../../Helper/common_headers.h"

class HdrBloom
{
    private:
        Shader shader;
        Model screenQuad, plane, teapot, sphere;

        GLuint hdrFbo, blurFbo;
        GLuint hdrTex, tex1, tex2;
        GLuint linearSampler, nearestSampler;

        GLint bloomBufWidth, bloomBufHeight;
        GLint width, height;

    public:
        HdrBloom(GLint width, GLint height);
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        void SetMatrics(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
        void SetupFBO();
        void Pass1(glm::mat4& view, glm::mat4& projection);
        void Pass2();
        void Pass3();
        void Pass4();
        void Pass5();
        float Guass(float x, float sigma2);
        void DrawScene(glm::mat4& view, glm::mat4& projection);
        void ComputeLogAveLumninace();

};