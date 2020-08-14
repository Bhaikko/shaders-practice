#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Frustum
{
    private:
        GLuint vao;
        glm::vec3 center, u, v, n;
        float mNear, mFar, fovy, ar;    // Field of view and Aspect Ration

        std::vector<GLuint> buffers;

    public:
        Frustum();
        ~Frustum();

        void Orient(const glm::vec3& pos, const glm::vec3& a, const glm::vec3& u);
        void SetPerspective(float fovy, float ar, float nearDist, float farDist);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetInverseViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        glm::vec3 GetOrigin() const;

        void Render();
        void DeleteBuffers();

};