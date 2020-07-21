#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include <GL/glew.h>

class Model
{
    public:
        Model();
        void LoadModel(const char* filePath);
        void RenderModel();
        ~Model();

    private:
        GLuint VAO, VBO, IBO;
        GLint verticesCount;
        GLfloat* meshData;

    private:
        void LoadModelDataIntoBuffers();
};