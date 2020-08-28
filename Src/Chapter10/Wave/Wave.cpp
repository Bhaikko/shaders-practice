#include "Wave.h"

Wave::Wave() 
{
    time = 0.0f;
    angle = glm::half_pi<float>();
}

void Wave::Init() 
{
    shader.CreateFromFiles(
        "./Src/Chapter10/Wave/shaders/Wave.vert",
        "./Src/Chapter10/Wave/shaders/Wave.frag"
    );

    plane.LoadModel("./Data/plane.obj");

    shader.UseShader();
    shader.SetUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));
    
}

void Wave::Render(glm::mat4& view, glm::mat4& projection) 
{
    shader.UseShader();

    shader.SetUniform("Time", time);

    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.5f, 0.3f));
    shader.SetUniform("Material.Ks", glm::vec3(0.8f, 0.8f, 0.8f));
    shader.SetUniform("Material.Ka", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.SetUniform("Material.Shininess", 100.0f);
    
    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);
    plane.RenderModel();
}

void Wave::Update(float deltaTime) 
{
    time = time + deltaTime;
    if (time >= 9999999999.0f) {
        time = 0.0f;
    }
}

void Wave::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}
