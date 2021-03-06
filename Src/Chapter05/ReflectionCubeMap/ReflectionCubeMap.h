#pragma once

#include "./../../../Helper/common_headers.h"

class ReflectionCubeMap
{
    private:
        Shader shader;
        Texture reflectionMap;
        Model teapot;

        float time;
        float angle, rotSpeed;
        float reflectionFactor;

    public:
        ReflectionCubeMap();
        void Init();
        void Update(float deltaTime);
        void Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition);
};