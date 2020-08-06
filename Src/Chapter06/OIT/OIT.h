#pragma once

#include "./../../../Helper/common_headers.h"

enum BufferName {
    COUNTER_BUFFER = 0,
    LINKED_LIST_BUFFER
};

struct ListNode {
    glm::vec4 color;
    GLfloat depth;
    GLuint next;
};

class OIT
{
    private:
        Shader shader;
        Model cube, sphere, screenQuad;

        GLuint buffers[2], headPtrTex;
        GLuint pass1Index, pass2Index;
        GLuint clearBuf;

        GLint height, width;

    public:
        OIT(GLint width, GLint height);
        void Init();
        void Render(glm::mat4& view, glm::mat4& projection);

    private:
        void SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
        void DrawScene(glm::mat4& view, glm::mat4& projection);
        void InitShaderStorage();
        void Pass1(glm::mat4& view, glm::mat4& projection);
        void Pass2();
        void ClearBuffers();

    
};