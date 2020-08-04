#pragma once

#include "./../../../Helper/common_headers.h"

class Deferred
{
    private:
        Shader shader;
        GLuint deferredFBO;

        Model screenQuad, plane, torus, teapot;

        GLint width, height;
    public:
        Deferred(GLint& width, GLint& height);
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        void SetMatrices(glm::mat4 model, glm::mat4& view, glm::mat4& projection);
        void SetupFbo();
        void CreateGBufferTex(GLenum textureUnit, GLenum format, GLuint& textureID);
        void Pass1(glm::mat4& view, glm::mat4& projection);
        void Pass2();
};  