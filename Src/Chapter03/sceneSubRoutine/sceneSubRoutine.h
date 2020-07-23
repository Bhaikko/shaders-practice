#pragma once

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./../../../Helper/Shader/Shader.h"
#include "./../../../Helper/Model/Model.h"

class SceneSubRoutine
{
    private:
        Shader shader;
        GLuint shaderID;
        Model model;


    public:
        SceneSubRoutine();
        void Init();
        void Render(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection 
        );
};