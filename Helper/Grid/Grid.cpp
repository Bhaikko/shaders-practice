#include "Grid.h"


Grid::Grid(float size, int nDivisions) 
{
    this->size = size;
    this->nDivisions = nDivisions;
}

Grid::~Grid() 
{
    if (buffers.size() > 0) {
        glDeleteBuffers(GLsizei(buffers.size()), buffers.data());
        buffers.clear();
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}

void Grid::Init() 
{
    float size2 = size / 2.0f;
    float divisionSize = size / nDivisions;

    nVerts = 4 * (nDivisions + 1);

    std::vector<GLfloat> p;

    GLfloat x, y = 0.0f, z;
    for (int row = 0; row <= nDivisions; row++) {
        z = (row * divisionSize) - size2;

        p.push_back(-size2);
        p.push_back(y);
        p.push_back(z);

        p.push_back(size2);
        p.push_back(y);
        p.push_back(z);
    }

    for (int col = 0; col <= nDivisions; col++) {
        x = (col * divisionSize) - size2;

        p.push_back(x);
        p.push_back(y);
        p.push_back(-size2);

        p.push_back(x);
        p.push_back(y);
        p.push_back(size2);
    }

    GLuint posBuf;
    glGenBuffers(1, &posBuf);
    buffers.push_back(posBuf);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glBufferData(GL_ARRAY_BUFFER, p.size() * sizeof(GLfloat), p.data(), GL_STATIC_DRAW);

    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    shader.CreateFromFiles(
        "./Helper/Grid/shaders/Grid.vert",
        "./Helper/Grid/shaders/Grid.frag"
    );

    shader.UseShader();
    shader.SetUniform("Color", glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));

}

void Grid::Render(glm::mat4& view, glm::mat4& projection) 
{
    shader.UseShader();

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);

    glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, nVerts);
    glBindVertexArray(0);
}

void Grid::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;

    shader.SetUniform("MV", mv);
    shader.SetUniform("Projection", projection);

}
