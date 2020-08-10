#pragma once 

#include "./../../../Helper/common_headers.h"

class WireFrame 
{
    private:
        Shader shader;
        Model ogre;

        GLint width, height;

        glm::mat4 viewportMatrix;

    public:
        WireFrame(GLint width, GLint height);
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
};