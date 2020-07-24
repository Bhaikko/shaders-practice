#include "ToonShading.h"

ToonShading::ToonShading()
{
    angle = 0.0f;
}

void ToonShading::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter04/ToonShading/shaders/ToonShading.vert",
        "./Src/Chapter04/ToonShading/shaders/ToonShading.frag"
    );

    torus.LoadModel("./Data/Torus.obj");
    bunny.LoadModel("./Data/bunny.obj");

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.La");
    shader.RegisterUniform("Light.L");

    shader.RegisterUniform("Material.Ka");
    shader.RegisterUniform("Material.Kd");

    shader.UseShader();

    glUniform3f(shader.GetUniformLocation("Light.L"), 0.9f, 0.9f, 0.9f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);
}

void ToonShading::Update(float deltaTime)
{
    angle += 0.25f * deltaTime;
    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }
}

void ToonShading::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    glm::vec4 lightPos = glm::vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    glUniform4f(shader.GetUniformLocation("Light.Position"), lightPos.x, lightPos.y, lightPos.z, lightPos.w);

    // Rendering Bunny
    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.9f, 0.5f, 0.3f);
    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.27f, 0.15f, 0.09f);

    glm::mat4 model(1.0f);
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    bunny.RenderModel();

    // Rendering Torus
    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.7f, 0.7f, 0.7f);
    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.2f, 0.2f, 0.2f);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    torus.RenderModel();

    

}