#include "ModelTess.h"

ModelTess::ModelTess(GLint width, GLint height)
{
    this->width = width;
    this->height = height;
}

void ModelTess::Init()
{
    sphere.LoadModel("./Data/cube.obj");

    shader.CreateFromFiles(
        "./Src/Chapter07/ModelTess/shaders/ModelTess.vert",
        "./Src/Chapter07/ModelTess/shaders/ModelTess.frag",
        "./Src/Chapter07/ModelTess/shaders/ModelTess.geom",
        "./Src/Chapter07/ModelTess/shaders/ModelTess.tese",
        "./Src/Chapter07/ModelTess/shaders/ModelTess.tesc"
    );

    shader.UseShader();

    shader.SetUniform("TessLevel", 8);
    shader.SetUniform("LineWidth", 0.8f);
    shader.SetUniform("LineColor", glm::vec4(0.05f, 0.0f, 0.05f, 1.0f));
    shader.SetUniform("LightPosition", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    shader.SetUniform("LightIntensity", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform("Kd", glm::vec3(0.9f, 0.9f, 1.0f));

    glPatchParameteri(GL_PATCH_VERTICES, 16);    

    float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    viewportMatrix = glm::mat4( 
        glm::vec4(w2,0.0f,0.0f,0.0f),
        glm::vec4(0.0f,h2,0.0f,0.0f),
        glm::vec4(0.0f,0.0f,1.0f,0.0f),
        glm::vec4(w2+0, h2+0, 0.0f, 1.0f)
    );

    shader.SetUniform("ViewportMatrix", viewportMatrix);
}

void ModelTess::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);
    sphere.DrawAsPatch(16);

    glFinish();
}

void ModelTess::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glm::mat4 mv = view * model;

    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]),
        glm::vec3(mv[1]),
        glm::vec3(mv[2])
    ));
}
