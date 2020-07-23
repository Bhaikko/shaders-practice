#include "Fog.h"

Fog::Fog()
{

}

void Fog::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter03/Fog/shaders/Fog.vert",
        "./Src/Chapter03/Fog/shaders/Fog.frag"
    );

    bunny.LoadModel("./Data/bunny.obj");
    terrain.LoadModel("./Data/terrain.obj");

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.La");
    shader.RegisterUniform("Light.L");

    shader.RegisterUniform("Material.Ka");
    shader.RegisterUniform("Material.Kd");
    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.RegisterUniform("Fog.MaxDistance");
    shader.RegisterUniform("Fog.MinDistance");
    shader.RegisterUniform("Fog.Color");

    shader.UseShader();

    glUniform3f(shader.GetUniformLocation("Light.L"), 0.9f, 0.9f, 0.9f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);
    
    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.27f, 0.15f, 0.09f);
    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.9f, 0.5f, 0.3f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.0f, 0.0f, 0.0f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 180.0f);
    
    glUniform1f(shader.GetUniformLocation("Fog.MaxDistance"), 30.0f);
    glUniform1f(shader.GetUniformLocation("Fog.MinDistance"), 1.0f);
    glUniform3f(shader.GetUniformLocation("Fog.Color"), 0.5f, 0.5f, 0.5f);

}

void Fog::Render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection 
) {
    shader.UseShader();

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    bunny.RenderModel();
    terrain.RenderModel();
}
    