#pragma once

#include "./../common_headers.h"

class Skybox
{
    private:
        Shader shader;
        Texture cubeMap;

        GLuint VAO;

    public:
        Skybox();
        void Init();
        void Render(glm::mat4 view, glm::mat4 projection);

    private:
        void InitCube();
    
};