#pragma once 

#include "./../../../Helper/common_headers.h"

class Fog
{
    public:
        Fog();
        void Init();
        void Render(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection 
        );

    private:
        Shader shader;
        Model bunny, terrain;

};