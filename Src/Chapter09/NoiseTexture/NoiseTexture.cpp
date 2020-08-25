#include "NoiseTexture.h"

#include "./../NoiseTex.h"

NoiseTexture::NoiseTexture() 
{
    
}

void NoiseTexture::Init() 
{
    shader.CreateFromFiles(
        "./Src/Chapter09/NoiseTexture/shaders/NoiseTexture.vert",
        "./Src/Chapter09/NoiseTexture/shaders/NoiseTexture.frag"
    );

    quad.LoadModel("./Data/plane.obj");

    GLuint noiseTex = NoiseTex::GeneratePeriodic2DTex();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);
}

void NoiseTexture::Render(glm::mat4& projection) 
{
    shader.UseShader();

    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ProjectionMatrix", projection);

    quad.RenderModel();
}
