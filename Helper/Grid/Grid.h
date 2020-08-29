#pragma once

#include "./../common_headers.h"

class Grid
{
private:
    std::vector<GLuint> buffers;
    GLuint VAO;
    GLsizei nVerts;

    Shader shader;

    float size;
    int nDivisions;

public:
    Grid(float size = 10.0f, int nDivisions = 10);
    ~Grid();

    void Init();
    void Render(glm::mat4& view, glm::mat4& projection);

private:
    void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

};