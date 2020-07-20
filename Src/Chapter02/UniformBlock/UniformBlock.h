#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./../../../Helper/Shader/Shader.h"

class UniformBlock
{
    public:
        UniformBlock();

        void Init();
        void Update(float t);
        void Render();


    private:
        GLuint VAO;
        GLuint shaderProgram;
        Shader* shader;
        float angle;

        void InitUniformBuffer();
};