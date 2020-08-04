#pragma once

#include "./../../../Helper/common_headers.h"

class MSAA
{
    private:
        Shader shader;
        Model plane;

    public:
        MSAA();
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
};