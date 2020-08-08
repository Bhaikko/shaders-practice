#pragma once 

#include "./../../../Helper/common_headers.h"
#include "Random.h"

class SSAO
{
    private:
        Shader shader;
        GLuint aoTex[2];

        GLuint deferredFBO, ssaoFBO, blurFBO;

        Random rand;

        Model plane, dragon, screenFillQuad;
        Texture woodTex, brickTex;

        GLint width, height;

    public:
        SSAO(GLint width, GLint height);
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        GLuint BuildRanRotationTex();
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
        void SetupFBO();
        void CreateGBuffTex(GLenum texUnit, GLenum format, GLuint& texID);
        void BuildKernel();

        void Pass1(glm::mat4& view, glm::mat4& projection);
        void Pass2();
        void Pass3();
        void Pass4();

        void DrawScreenFillingQuad();
        void DrawScene(glm::mat4& view, glm::mat4& projection);
};