#include "BezierCurve.h"

BezierCurve::BezierCurve()
{

}

void BezierCurve::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter07/BezierCurve/shaders/BezierCurve.vert",
        "./Src/Chapter07/BezierCurve/shaders/BezierCurve.frag",
        "",
        "./Src/Chapter07/BezierCurve/shaders/BezierCurve.tese",
        "./Src/Chapter07/BezierCurve/shaders/BezierCurve.tesc"
    );

    solidShader.CreateFromFiles(
        "./Src/Chapter07/BezierCurve/shaders/solid.vert",
        "./Src/Chapter07/BezierCurve/shaders/solid.frag"
    );

    float c = 3.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
    glPointSize(10.0f);

    glm::vec3 cameraPos(0.0f ,0.0f,1.5f);
    view = glm::lookAt(
        cameraPos,
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f)
    );

    // Setup Patch VBO with (x, y) coordinates
    GLfloat v[] = {
        -1.0f, -1.0f, 
        -0.5f, 1.0f,  
         0.5f, -1.0f, 
         1.0f, 1.0f
    };

    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), v, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);


    // Set the number of vertices per patch
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    shader.UseShader();
    shader.SetUniform("NumSegments", 10);
    shader.SetUniform("NumStrips", 1);
    shader.SetUniform("LineColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    solidShader.UseShader();
    solidShader.SetUniform("Color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void BezierCurve::Render()
{
    glm::mat4 model(1.0f);

    glBindVertexArray(vaoHandle);
    SetMatrices(model);
     
    shader.UseShader();
    glDrawArrays(GL_PATCHES, 0, 4);

    solidShader.UseShader();
    glDrawArrays(GL_POINTS, 0, 4);

    glFinish();
}


void BezierCurve::SetMatrices(glm::mat4& model)
{
    glm::mat4 mv = view * model;
    shader.UseShader();
    shader.SetUniform("MVP", projection * mv);
    solidShader.UseShader();
    solidShader.SetUniform("MVP", projection * mv);
}
