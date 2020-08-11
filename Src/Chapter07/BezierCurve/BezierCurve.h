#pragma once

#include "./../../../Helper/common_headers.h"

class BezierCurve
{
    private:
        Shader shader, solidShader;
        GLuint vaoHandle;

        glm::mat4 view, projection;

    public:
        BezierCurve();
        void Init();
        void Render();

    private:
        void SetMatrices(glm::mat4& model);
};