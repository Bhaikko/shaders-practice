#pragma once

#include "./../../../Helper/common_headers.h"

class QuadTess
{
    private:
        Shader shader;
        GLuint vaoHandle;

        glm::mat4 view, projection, viewport;
    
        GLint width, height;

    public:
        QuadTess(GLint width, GLint height);
        void Init();
        void Render();

    private:
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

    
};