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
        void RenderAsAdjacency();
        void DrawAsPatch(GLint patchSize);
        ~Model();

    private:
        GLuint VAO;
        GLuint positionBufferID, normalBufferID, textureBufferID;
    
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec4> tangents;
        std::vector<FaceIndex> faces;

    private:
        void LoadModelDataIntoBuffers();

    public:
        GLuint GetVaoID();
        GLuint GetPositionBuffer() { return this->positionBufferID; }
        GLuint GetTextureBuffer() { return this->textureBufferID; }
        GLuint GetNormalBuffer() { return this->normalBufferID; }
        GLuint GetNumberOfVertices() { return this->faces.size(); }
};