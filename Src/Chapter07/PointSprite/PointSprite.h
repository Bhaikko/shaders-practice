#pragma once

#include "./../../../Helper/common_headers.h"

class PointSprite
{
    private:
        Shader shader;
        Texture flower;

        GLuint sprites;
        int numSprites;
        GLfloat* locations;

    public:
        PointSprite();
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
};