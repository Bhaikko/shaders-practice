#include "Wireframe.h"

WireFrame::WireFrame(GLint width, GLint height)
{
    this->width = width;
    this->height = height;

    viewportMatrix = glm::mat4(
        glm::vec4(width / 2.0f, 0.0f,           0.0f, 0.0f),
        glm::vec4(0.0f,         height / 2.0f,  0.0f, 0.0f),
        glm::vec4(0.0f,         0.0f,           1.0f, 0.0f),
        glm::vec4(width / 2.0f, height / 2.0f,  0.0f, 1.0f)
    );
}

void WireFrame::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter07/Wireframe/shaders/Wireframe.vert",
        "./Src/Chapter07/Wireframe/shaders/Wireframe.frag",
        "./Src/Chapter07/Wireframe/shaders/Wireframe.geom"
    );

    ogre.LoadModel("./Data/teapot.obj");

    shader.UseShader();
    shader.SetUniform("Line.Width", 0.75f);
    shader.SetUniform("Line.Color", glm::vec4(0.05f, 0.0f, 0.05f, 1.0f));

    shader.SetUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    shader.SetUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));

    shader.SetUniform("Material.Kd", glm::vec3(0.7f, 0.7f, 0.7f));
    shader.SetUniform("Material.Ka", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.SetUniform("Material.Ks", glm::vec3(0.8f, 0.8f, 0.8f));
    shader.SetUniform("Material.Shininess", 100.0f);

    shader.SetUniform("ViewportMatrix", viewportMatrix);

}

void WireFrame::Render(glm::mat4& view, glm::mat4& projection)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);
    ogre.RenderModel();
}


void WireFrame::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
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
