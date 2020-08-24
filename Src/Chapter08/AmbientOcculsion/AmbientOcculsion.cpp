#include "AmbientOcculsion.h"

AO::AO() 
{
    lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    angle = 0.0f;
    rotSpeed = 5.0f;
}

void AO::Init() 
{
    shader.CreateFromFiles(
        "./Src/Chapter08/AmbientOcculsion/shaders/AmbientOcculsion.vert",
        "./Src/Chapter08/AmbientOcculsion/shaders/AmbientOcculsion.frag"
    );

    ogre.LoadModel("./Data/bs_ears.obj");

    diffuseTex.LoadTexture("./Data/Textures/ogre_diffuse.png");
    aoTex.LoadTexture("./Data/Textures/ao.png", GL_RGB);

    shader.UseShader();
    shader.SetUniform("Light.Position", lightPos);
    shader.SetUniform("Light.Intensity", glm::vec3(1.0f));

}

void AO::Render(glm::mat4& view, glm::mat4& projection) 
{
    diffuseTex.UseTexture(GL_TEXTURE1);
    aoTex.UseTexture(GL_TEXTURE0);

    shader.UseShader();

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);
    ogre.RenderModel();
}

void AO::Update(float deltaTime) 
{
    angle += rotSpeed * deltaTime;

    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }    
}

void AO::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
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