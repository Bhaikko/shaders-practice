#pragma once 

#include "./../../../Helper/common_headers.h"

class ModelTess
{
    private:
        Model sphere;
        Shader shader;

        GLint height, width;
        glm::mat4 viewportMatrix;

    public:
        ModelTess(GLint width, GLint height);
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

        
};