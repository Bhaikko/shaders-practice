#include "AlphaTest.h"


AlphaTest::AlphaTest()
{
    angle = 0.0f;
    rotSpeed = glm::pi<float>() / 2.0f;
}

void AlphaTest::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter05/AlphaTest/shaders/AlphaTest.vert",
        "./Src/Chapter05/AlphaTest/shaders/AlphaTest.frag"
    );

    moss.LoadTexture("./Data/Textures/moss.png");
    cement.LoadTexture("./Data/Textures/cement.png");
    
    teapot.LoadModel("./Data/teapot.obj");
    teapot.GenerateTangents();

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("BaseTex");
    shader.RegisterUniform("AlphaTex");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.L");
    shader.RegisterUniform("Light.La");

    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.UseShader();

    glUniform4f(shader.GetUniformLocation("Light.Position"), 0.0f, 0.0f, 0.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.L"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);

    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.0f, 0.0f, 0.0f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 1.0f);

    glUniform1i(shader.GetUniformLocation("BaseTex"), 0);
    glUniform1i(shader.GetUniformLocation("AlphaTex"), 1);

}

void AlphaTest::Update(float deltaTime)
{
    angle += rotSpeed * deltaTime;
    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }
}

void AlphaTest::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(0.1f));
    // model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));

    cement.UseTexture(GL_TEXTURE0);
    moss.UseTexture(GL_TEXTURE1);

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    teapot.RenderModel();
}
