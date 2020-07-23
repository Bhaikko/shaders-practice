#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./../../../Helper/Shader/Shader.h"
#include "./../../../Helper/Model/Model.h"

class SceneDiscard
{
    public:
        SceneDiscard();

        void Init();
        void Render(
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection
        );

    private:
        Shader shader;
        Model model;

};