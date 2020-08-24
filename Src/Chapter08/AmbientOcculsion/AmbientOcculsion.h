#pragma once

#include "./../../../Helper/common_headers.h"

class AO
{
private:
    Shader shader;
    Model ogre;
    Texture diffuseTex, aoTex;

    glm::vec4 lightPos;
    float angle, rotSpeed;

public:
    AO();
    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);
    void Update(float deltaTime);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
};