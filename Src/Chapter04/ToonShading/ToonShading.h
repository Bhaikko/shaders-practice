#pragma once

#include "./../../../Helper/common_headers.h"

class ToonShading
{
    private:
        Shader shader;
        Model torus, bunny;
        float angle;

    public:
        ToonShading();
        void Init();
        void Update(float deltaTime);
        void Render(glm::mat4 view, glm::mat4 projection);
};