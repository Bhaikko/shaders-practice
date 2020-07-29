#include "Parallax.h"


Parallax::Parallax()
{

}

void Parallax::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter05/Parallax/shaders/Parallax.vert",
        "./Src/Chapter05/Parallax/shaders/Parallax.frag"
    );

    plane.LoadModel("./Data/plane.obj");
    colorMap.LoadTexture("./Data/Textures/mybrick/mybrick-color.png");
    normalMap.LoadTexture("./Data/Textures/mybrick/mybrick-normal.png");
    heightMap.LoadTexture("./Data/Textures/mybrick/mybrick-height.png");

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.L");
    shader.RegisterUniform("Light.La");

    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.UseShader();

    glUniform4f(shader.GetUniformLocation("Light.Position"), 2.0f, 2.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.L"), 0.7f, 0.7f, 0.7f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.07f, 0.07f, 0.07f);

    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.7f, 0.7f, 0.7f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 40.0f);



    glEnable(GL_MULTISAMPLE);
}

void Parallax::Update(float deltaTime)
{

}

void Parallax::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    colorMap.UseTexture(GL_TEXTURE0);
    normalMap.UseTexture(GL_TEXTURE1);
    heightMap.UseTexture(GL_TEXTURE2);

    glm::mat4 model(1.0f);
    model = glm::rotate(glm::mat4(1.0f), glm::radians(65.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    plane.RenderModel();
}
