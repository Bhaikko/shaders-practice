#pragma once 

#include "./../../../Helper/common_headers.h"

class AlphaTest
{
    private:
        Shader shader;
        Model teapot;
        Texture cement, moss;

        float angle, rotSpeed;

    public:
        AlphaTest();
        void Init();
        void Update(float deltaTime);
        void Render(glm::mat4 view, glm::mat4 projection);

};