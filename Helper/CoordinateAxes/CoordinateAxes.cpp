#include "CoordinateAxes.h"

CoordinateAxes::CoordinateAxes()
{
    GLfloat tempVertices[12] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    std::memcpy(vertices, tempVertices, sizeof(tempVertices));

    xIndices[0] = 0;
    xIndices[1] = 1;

    yIndices[0] = 0;
    yIndices[1] = 2;

    zIndices[0] = 0;
    zIndices[1] = 3;

}

void CoordinateAxes::InitialiseCoordinateAxes()
{
    AllocateBufferObjects(xVAO, xVBO, xIBO, xIndices);
    AllocateBufferObjects(yVAO, yVBO, yIBO, yIndices);
    AllocateBufferObjects(zVAO, zVBO, zIBO, zIndices);
}

void CoordinateAxes::AllocateBufferObjects(GLuint &VAO, GLuint &VBO, GLuint &IBO, unsigned int indices[])
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof (indices[0]), indices, GL_STATIC_DRAW);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    
}

void CoordinateAxes::RenderCoordinateAxes()
{
    DrawSingleCoordinateAxis(xVAO, xIBO);
    DrawSingleCoordinateAxis(yVAO, yIBO);
    DrawSingleCoordinateAxis(zVAO, zIBO);
}

void CoordinateAxes::DrawSingleCoordinateAxis(GLuint &VAO, GLuint &IBO)
{
    glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glLineWidth(100.0f);
            glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

CoordinateAxes::~CoordinateAxes()
{
    
}