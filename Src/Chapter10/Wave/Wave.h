#pragma once

#include "./../../../Helper/common_headers.h"

class Wave
{
private:
    Shader shader;
    Model plane;

    float angle, time;

public:
    Wave();
    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);
    void Update(float deltaTime);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
};