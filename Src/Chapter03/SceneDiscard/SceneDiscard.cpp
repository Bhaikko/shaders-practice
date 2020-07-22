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

    shaderID = shader.GetShaderProgramID();

    model.LoadModel("./Data/sphere.obj");

    glEnable(GL_DEPTH_TEST);

    Light.Position = glGetUniformLocation(shaderID, "Light.Position");
    Light.La = glGetUniformLocation(shaderID, "Light.La");
    Light.Ld = glGetUniformLocation(shaderID, "Light.Ld");
    Light.Ls = glGetUniformLocation(shaderID, "Light.Ls");

    Material.Ka = glGetUniformLocation(shaderID, "Material.Ka");
    Material.Kd = glGetUniformLocation(shaderID, "Material.Kd");
    Material.Ks = glGetUniformLocation(shaderID, "Material.Ks");
    Material.Shininess = glGetUniformLocation(shaderID, "Material.Shininess");

    ModelMatrix = glGetUniformLocation(shaderID, "ModelMatrix");
    ViewMatrix = glGetUniformLocation(shaderID, "ViewMatrix");
    ProjectionMatrix = glGetUniformLocation(shaderID, "ProjectionMatrix");
    NormalMatrix = glGetUniformLocation(shaderID, "NormalMatrix");


    glUniform3f(Material.Kd, 0.9f, 0.5f, 0.3f);
    glUniform3f(Light.Ld, 1.0f, 1.0f, 1.0f);

    glUniform3f(Material.Ka, 0.9f, 0.5f, 0.3f);
    glUniform3f(Light.La, 0.4f, 0.4f, 0.4f);

    glUniform3f(Material.Ks, 0.8f, 0.8f, 0.8f);
    glUniform3f(Light.Ls, 1.0f, 1.0f, 1.0f);

    glUniform1f(Material.Shininess, 100.0f);



}

void SceneDiscard::Render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
)
{
    shader.UseShader();

    glUniform4f(Light.Position, 0.0f,0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(ProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));

    // Normal Matrix Calculation
    glm::mat4 mv = view * model;
    glm::mat4 normalMatrix4fv = glm::transpose(glm::inverse(mv));
    glm::mat3 normalMatrix = glm::mat3(glm::vec3(normalMatrix4fv[0]), glm::vec3(normalMatrix4fv[1]), glm::vec3(normalMatrix4fv[2]));


    glUniformMatrix3fv(NormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    this->model.RenderModel();
}
