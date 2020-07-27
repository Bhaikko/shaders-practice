#include "TextureProjection.h"

TextureProjection::TextureProjection()
{

}

void TextureProjection::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter05/TextureProjection/shaders/TextureProjection.vert",
        "./Src/Chapter05/TextureProjection/shaders/TextureProjection.frag"
    );

    bunny.LoadModel("./Data/teapot.obj");
    terrain.LoadModel("./Data/terrain.obj");
    flower.LoadTexture("./Data/Textures/flower.png");

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.L");
    shader.RegisterUniform("Light.La");

    shader.RegisterUniform("Material.Ka");
    shader.RegisterUniform("Material.Kd");
    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.UseShader();

    glUniform4f(shader.GetUniformLocation("Light.Position"), 0.0f, 0.0f, 0.0, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.L"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);

    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.1f, 0.1f, 0.1f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.95f, 0.95f, 0.95f);
    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.5f, 0.2f, 0.1f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 100.0f);
}

void TextureProjection::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    
    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    flower.UseTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    bunny.RenderModel();


    model = glm::mat4(1.0f);
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    terrain.RenderModel();
}
