#pragma once 

#include "./../../../Helper/common_headers.h"

class TextureProjection
{
    private:
        Model bunny, terrain;
        Shader shader;
        Texture flower;

    public:
        TextureProjection();
        void Init();
        void Render(glm::mat4 view, glm::mat4 projection);
};