#pragma once

#include "./../../../Helper/common_headers.h"

class SamplerObject
{
    private:
        Shader shader;
        Model plane;

        GLuint linearSampler, nearestSampler;

        float angle;

    public:
        SamplerObject();
        void Init();
        void Update(float deltaTime);
        void Render(glm::mat4 view, glm::mat4 projection);

    private:
        void CreateTexture();

};