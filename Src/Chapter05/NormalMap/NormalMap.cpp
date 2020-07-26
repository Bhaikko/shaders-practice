#include "NormalMap.h"

NormalMap::NormalMap()
{
    angle = 0.0f;
    rotSpeed = glm::pi<float>() / 8.0f;
}

void NormalMap::Init()
{
    head.LoadModel("./Data/bs_ears.obj");
    head.GenerateTangents();

    baseTexture.LoadTexture("./Data/Textures/ogre_diffuse.png");
    normalMap.LoadTexture("./Data/Textures/ogre_normalmap.png");

    shader.CreateFromFiles(
        "./Src/Chapter05/NormalMap/shaders/NormalMap.vert",
        "./Src/Chapter05/NormalMap/shaders/NormalMap.frag"
    );

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("NormalMatrix");
    shader.RegisterUniform("ProjectionMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.L");
    shader.RegisterUniform("Light.La");

    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.UseShader();
    glUniform3f(shader.GetUniformLocation("Light.L"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);

    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.2f, 0.2f, 0.2f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 1.0f);

}

void NormalMap::Update(float deltaTime)
{
    angle += rotSpeed * deltaTime;
    if (angle > glm::two_pi<float>()) { 
        angle -= glm::two_pi<float>();
    }
}

void NormalMap::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    glm::mat4 model(1.0f);

    glm::vec4 lightPos = view * glm::vec4(
        10.0f * cos(angle),
        1.0f,
        10.0f * sin(angle),
        1.0f
    );

    glUniform4f(shader.GetUniformLocation("Light.Position"), lightPos.x, lightPos.y, lightPos.z, lightPos.w);


    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    baseTexture.UseTexture(GL_TEXTURE0);
    normalMap.UseTexture(GL_TEXTURE1);

    head.RenderModel();
}
