#pragma once 

#include "./../../../Helper/common_headers.h"

class EdgeDetection
{
    private:
        Shader shader;
        Model torus, plane, teapot;
        Model screenQuad;

        GLuint fboHandle, renderTex;
        GLuint width, height;

    public:
        EdgeDetection(GLint width, GLint height);
        void Init();
        void Render(glm::mat4 view, glm::mat4 projection);

    private:
        void SetupFBO();
        void Pass1(glm::mat4 view, glm::mat4 projection);
        void Pass2();
};