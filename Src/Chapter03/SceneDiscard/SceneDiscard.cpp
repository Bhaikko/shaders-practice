#include "SceneDiscard.h"

SceneDiscard::SceneDiscard()
{
}

void SceneDiscard::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter03/SceneDiscard/shaders/sceneDiscard.vert",
        "./Src/Chapter03/SceneDiscard/shaders/sceneDiscard.frag"
    );

    model.LoadModel("./Data/sphere.obj");

    glEnable(GL_DEPTH_TEST);

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");

}

void SceneDiscard::Render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
)
{
    shader.UseShader();


    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));

    this->model.RenderModel();
}
