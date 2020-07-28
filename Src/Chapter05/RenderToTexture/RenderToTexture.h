#pragma once 

#include "./../../../Helper/common_headers.h"

class RenderToTexture
{
    private:
        Shader shader;
        Model cow, cube;
        Texture texture;

        float angle, rotSpeed;
        GLuint fboHandle;

    public:
        RenderToTexture();
        void Init();
        void Render(glm::mat4 view, glm::mat4 projection);
        void Update(float deltaTime);

    private:
        void SetupFBO();
        void RenderTexture();
        void RenderScene(glm::mat4 view, glm::mat4 projection);
};