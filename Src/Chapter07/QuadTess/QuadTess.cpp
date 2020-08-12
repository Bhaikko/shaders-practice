#include "QuadTess.h"

QuadTess::QuadTess(GLint width, GLint height)
{
    this->width = width;
    this->height = height;
}

void QuadTess::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter07/QuadTess/shaders/QuadTess.vert",
        "./Src/Chapter07/QuadTess/shaders/QuadTess.frag",
        "./Src/Chapter07/QuadTess/shaders/QuadTess.geom",
        "./Src/Chapter07/QuadTess/shaders/QuadTess.tese",
        "./Src/Chapter07/QuadTess/shaders/QuadTess.tesc"
    );

    glm::vec3 cameraPos(0.0f,0.0f,1.5f);
    view = glm::lookAt(
        cameraPos,
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f)
    );

    float c = 3.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);

    float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    viewport = glm::mat4( 
        glm::vec4(w2,0.0f,0.0f,0.0f),
        glm::vec4(0.0f,h2,0.0f,0.0f),
        glm::vec4(0.0f,0.0f,1.0f,0.0f),
        glm::vec4(w2+0, h2+0, 0.0f, 1.0f)
    );

    float v[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), v, GL_STATIC_DRAW);

        // Set up patch VAO
        glGenVertexArrays(1, &vaoHandle);
        glBindVertexArray(vaoHandle);

        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

    // Set the number of vertices per patch.  IMPORTANT!!
    glPatchParameteri( GL_PATCH_VERTICES, 4);

    shader.UseShader();

    shader.SetUniform("Inner", 8);
    shader.SetUniform("Outer", 8);
    shader.SetUniform("LineWidth", 1.5f);
    shader.SetUniform("LineColor", glm::vec4(0.05f, 0.0f, 0.05f, 1.0f));
    shader.SetUniform("QuadColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

}

void QuadTess::Render()
{
    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_PATCHES, 0, 4);

    glFinish();
}


void QuadTess::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glm::mat4 mv = view * model;
    shader.UseShader();
    shader.SetUniform("MVP", projection * mv);
    shader.SetUniform("ViewportMatrix", viewport);
}
