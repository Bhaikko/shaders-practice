#pragma once

#include "./../../../Helper/common_headers.h"

class NormalMap
{
    private:
        Model head;
        Texture baseTexture, normalMap, aoTex;
        Shader shader;

        float angle, rotSpeed;

    public:
        NormalMap();
        void Init();
        void Update(float deltaTime);
        void Render(glm::mat4 view, glm::mat4 projection);

};