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
        GLuint shaderID;

        Model model;

    private:
        // Uniform Values
        struct LightInfo {
            GLuint Position;
            GLuint La;
            GLuint Ld;
            GLuint Ls;
        } Light;

        struct MaterialInfo {
            GLuint Ka;
            GLuint Kd;
            GLuint Ks;
            GLuint Shininess;
        } Material;

        GLuint ModelMatrix, ViewMatrix, ProjectionMatrix, NormalMatrix;

};