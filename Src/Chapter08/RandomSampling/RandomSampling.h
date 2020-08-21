#pragma once

#include <random>
#include "./../../../Helper/common_headers.h"
#include "./../../../Helper/Frustum/Frustum.h"

class RandomSampling
{
    private:
        Shader shader;
        GLuint shadowFBO, pass1Index, pass2Index;

        Model plane, building;
        Frustum lightFrustum;

        GLint samplesU, samplesV;
        int jitterMapSize;
        float radius;
        GLint shadowMapWidth, shadowMapHeight;
        GLint width, height;

        glm::mat4 lightPOV, shadowScale;
        glm::vec3 lightPos;

        
    public:
        RandomSampling(GLint width, GLint height);

        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
        void SetupFBO();
        void DrawBuildingScene(glm::mat4& view, glm::mat4& projection);
        float Jitter();
        void BuildJitterTex();

};