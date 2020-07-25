#pragma once

#include <cstring>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./../Shader/Shader.h"

class CoordinateAxes
{
    public:
        CoordinateAxes();
        void InitialiseCoordinateAxes();
        void RenderCoordinateAxes(
            glm::mat4 model,
            glm::mat4 projection,
            glm::mat4 view
        );
        ~CoordinateAxes();

    private:
        Shader shader;

        GLuint xVAO, xVBO, xIBO;
        GLuint yVAO, yVBO, yIBO;
        GLuint zVAO, zVBO, zIBO;
        GLfloat vertices[12];
        unsigned int xIndices[2], yIndices[2], zIndices[2];

        void AllocateBufferObjects(GLuint &VAO, GLuint &VBO, GLuint &IBO, unsigned int indices[]);
        void DrawSingleCoordinateAxis(GLuint &VAO, GLuint &IBO);

};