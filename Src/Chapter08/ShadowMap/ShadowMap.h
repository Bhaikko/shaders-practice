#pragma once 

#include "./../../../Helper/common_headers.h"
#include "./../../../Helper/Frustum/Frustum.h"

class ShadowMap
{
    private:
        Shader shader, solidShader;
        GLuint shadowFBO, pass1Index, pass2Index;

        Model teapot, plane, torus;

        GLint shadowMapWidth, shadowMapHeight;
        GLint width, height;

        glm::mat4 lightPOV, shadowBias;

        Frustum lightFrustum;

        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
        void SetupFBO();
        void DrawScene(glm::mat4& view, glm::mat4& projection);
        void SpitoutDepthBuffer();

    public:
        ShadowMap(GLint width, GLint height);

        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);
};