#include "Frustum.h"

Frustum::Frustum()
{
    this->Orient(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->SetPerspective(50.0f, 1.0f, 0.5f, 100.0f);
}

Frustum::~Frustum()
{
    DeleteBuffers();
}

void Frustum::Orient(const glm::vec3& pos, const glm::vec3& at, const glm::vec3& up)
{
    n = glm::normalize(pos - at);
    u = glm::normalize(glm::cross(up, n));
    v = glm::normalize(glm::cross(n, u));

    center = pos;
}

void Frustum::SetPerspective(float fovy, float ar, float nearDist, float farDist)
{
    this->DeleteBuffers();
    this->fovy = fovy;
    this->ar = ar;
    this->mNear = nearDist;
    this->mFar = farDist;

    float dy = mNear * tanf(glm::radians(fovy) / 2.0f);
    float dx = ar * dy;

    float fdy = mFar * tanf(glm::radians(fovy) / 2.0f);
    float fdx = ar * fdy;

    // Frustum Vertices
    std::vector<GLfloat> vert = {
         0.0f, 0.0f, 0.0f,
         dx,   dy,  -mNear, 
        -dx,   dy,  -mNear, 
        -dx,  -dy,  -mNear, 
         dx,  -dy,  -mNear,
         fdx,  fdy, -mFar, 
        -fdx,  fdy, -mFar, 
        -fdx, -fdy, -mFar, 
         fdx, -fdy, -mFar
    };

    std::vector<GLuint> indices = {
        0, 5,
        0, 6, 
        0, 7,
        0, 8,

        // The near plane
        1, 2,
        2, 3,
        3, 4,
        4, 1,

        // Far plane
        5, 6,
        6, 7,
        7, 8,
        8, 5
    };

    GLuint posBuff;
    glGenBuffers(1, &posBuff);
    buffers.push_back(posBuff);

    glBindBuffer(GL_ARRAY_BUFFER, posBuff);
    glBufferData(GL_ARRAY_BUFFER, 9 * 3 * sizeof(GLfloat), vert.data(), GL_STATIC_DRAW);

    GLuint indicesBuff;
    glGenBuffers(1, &indicesBuff);
    buffers.push_back(indicesBuff);

    glBindBuffer(GL_ARRAY_BUFFER, indicesBuff);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, posBuff);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuff);
    glBindVertexArray(0);

}

#include <iostream>

glm::mat4 Frustum::GetViewMatrix() const
{
    glm::mat4 rot(
        u.x, v.x, n.x, 0,
        u.y, v.y, n.y, 0,
        u.z, v.z, n.z, 0,
        0, 0, 0, 1
    );

    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(-center.x, -center.y, -center.z));

    return rot * trans;
}

glm::mat4 Frustum::GetInverseViewMatrix() const
{
    glm::mat4 rot (
        u.x, u.y, u.z, 0,
        v.x, v.y, v.z, 0,
        n.x, n.y, n.z, 0,
        0, 0, 0, 1
    );

    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(center.x, center.y, center.z));

    return trans * rot;
}

glm::mat4 Frustum::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(fovy), ar, mNear, mFar);
}

glm::vec3 Frustum::GetOrigin() const
{
    return this->center;
}


void Frustum::Render()
{
    glBindVertexArray(vao);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Frustum::DeleteBuffers()
{
    if (!buffers.empty()) {
        glDeleteBuffers((GLsizei)buffers.size(), buffers.data());
        buffers.clear();
    }

    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}
