#pragma once

#include "./../../../Helper/common_headers.h"

class NoiseTexture
{
private:
    Shader shader;
    Model quad;

public:
    NoiseTexture();
    void Init();
    void Render(glm::mat4& projection);

};