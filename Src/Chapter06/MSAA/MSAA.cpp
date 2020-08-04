#include "MSAA.h"

MSAA::MSAA()
{

}

void MSAA::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter06/MSAA/shaders/MSAA.vert",
        "./Src/Chapter06/MSAA/shaders/MSAA.frag"
    );

    plane.LoadModel("./Data/plane.obj");

    GLint bufs, samples;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    glGetIntegerv(GL_SAMPLES, &samples);

    printf("MSAA: buffers = %d samples = %d\n", bufs, samples);
    glEnable(GL_MULTISAMPLE);


}

void MSAA::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();

    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    SetMatrices(model, view, projection);
    plane.RenderModel();
}


void MSAA::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(glm::inverseTranspose(view * model)));
}
