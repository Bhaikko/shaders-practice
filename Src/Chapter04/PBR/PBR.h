#pragma once 

#include "./../../../Helper/common_headers.h"


// Physically based rendering (PBR) is an approach in computer graphics that seeks to render graphics in a way that more accurately models the flow of light in the real world
class PBR
{
    public:
        PBR();
        void Init();
        void Render(
            glm::mat4 view,
            glm::mat4 projection
        );
        void Update(float t);

    private:
        Shader shader;
        Model terrain, sphere;

        glm::vec4 lightPos;

        float lightAngle, lightRotationSpeed;
        unsigned int numBunny;

    private:
        void DrawFloor(glm::mat4 view, glm::mat4 projection);
        void DrawSpot(const glm::vec3& pos, float rough, int metal, const glm::vec3& color, glm::mat4 view, glm::mat4 projection);
};