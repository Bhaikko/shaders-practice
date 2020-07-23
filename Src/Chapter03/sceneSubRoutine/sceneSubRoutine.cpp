#include "sceneSubRoutine.h"

SceneSubRoutine::SceneSubRoutine()
{

}

void SceneSubRoutine::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter03/sceneSubRoutine/shaders/sceneSubRoutine.vert",
        "./Src/Chapter03/sceneSubRoutine/shaders/sceneSubRoutine.frag"
    );
    shaderID = shader.GetShaderProgramID();

    model.LoadModel("./Data/bunny.obj");
    glEnable(GL_DEPTH_TEST);

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.La");
    shader.RegisterUniform("Light.Ld");
    shader.RegisterUniform("Light.Ls");
    shader.RegisterUniform("Material.Ka");
    shader.RegisterUniform("Material.Kd");
    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    // Active program should be current shader before setting up Shader uniform values
    shader.UseShader();

    glUniform4f(shader.GetUniformLocation("Light.Position"), 1.0f, 0.0f, 0.0f, 1.0f);

    glUniform3f(shader.GetUniformLocation("Light.La"), 0.4f, 0.4f, 0.4f);
    glUniform3f(shader.GetUniformLocation("Light.Ld"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.Ls"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("color"), 1.0f, 1.0f, 1.0f);

    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.9f, 0.5f, 0.3f);
    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.9f, 0.5f, 0.3f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.8f, 0.8f, 0.8f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 100.0f);


}

void SceneSubRoutine::Render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection 
) {
    shader.UseShader();

    // Getting Function Indexes defined by Subroutine
    GLuint phongModelIndex = glGetSubroutineIndex(shaderID, GL_VERTEX_SHADER, "PhongModel");
    GLuint diffuseModelIndex = glGetSubroutineIndex(shaderID, GL_VERTEX_SHADER, "DiffuseModel");

    // Load active subroutine, PhongModel()
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &phongModelIndex);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0,-1.5,0.0));
    // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));

    // Setting up normal matrix
    glm::mat4 mv = view * model;
    glm::mat4 temp = glm::transpose(glm::inverse(mv));
    glm::mat3 normalMatrix = glm::mat3(
        glm::vec3(temp[0]),
        glm::vec3(temp[1]),
        glm::vec3(temp[2])
    );

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    this->model.RenderModel();

    // Load Active subroutine, DiffuseModel()
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &diffuseModelIndex);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f));
    // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));

    mv = view * model;
    temp = glm::transpose(glm::inverse(mv));
    normalMatrix = glm::mat3(
        glm::vec3(temp[0]),
        glm::vec3(temp[1]),
        glm::vec3(temp[2])
    );

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    this->model.RenderModel();

}