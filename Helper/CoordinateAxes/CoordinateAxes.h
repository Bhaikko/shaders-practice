#pragma once

#include <cstring>
#include <iostream>

#include <GL/glew.h>

class CoordinateAxes
{
    public:
        CoordinateAxes();
        void InitialiseCoordinateAxes();
        void RenderCoordinateAxes();
        ~CoordinateAxes();

    private:
        GLuint xVAO, xVBO, xIBO;
        GLuint yVAO, yVBO, yIBO;
        GLuint zVAO, zVBO, zIBO;
        GLfloat vertices[12];
        unsigned int xIndices[2], yIndices[2], zIndices[2];

        void AllocateBufferObjects(GLuint &VAO, GLuint &VBO, GLuint &IBO, unsigned int indices[]);
        void DrawSingleCoordinateAxis(GLuint &VAO, GLuint &IBO);
};