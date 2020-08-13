#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include <GL/glew.h>

struct FaceIndex {
    GLuint vertexIndex;
    GLuint textureCoordIndex;
    GLuint normalIndex;
};

class Model
{
    public:
        Model();
        void LoadModel(const char* filePath);
        void GenerateTangents();
        void RenderModel();
        void DrawAsPatch(GLint patchSize);
        ~Model();

    private:
        GLuint VAO;
    
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec4> tangents;
        std::vector<FaceIndex> faces;

    private:
        void LoadModelDataIntoBuffers();
};