#include "NightVision.h"
#include "./../NoiseTex.h"

NightVision::NightVision(GLint width, GLint height) 
{
    this->width = width;
    this->height = height;  
}

void NightVision::Init() 
{
    shader.CreateFromFiles(
        "./Src/Chapter09/NightVision/shaders/NightVision.vert",
        "./Src/Chapter09/NightVision/shaders/NightVision.frag"
    );

    screenQuad.LoadModel("./Data/plane.obj");
    plane.LoadModel("./Data/plane.obj");
    torus.LoadModel("./Data/Torus.obj");
    teapot.LoadModel("./Data/teapot.obj");

    SetupFBO();

    // Subroutine Setup
    GLuint programHandle = shader.GetShaderProgramID();
    pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "Pass1");
    pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "Pass2");

    shader.UseShader();
    shader.SetUniform("Width", width);
    shader.SetUniform("Height", height);
    shader.SetUniform("Radius", width / 3.5f);
    shader.SetUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));

    noiseTex = NoiseTex::GeneratePeriodic2DTex(200.0f, 0.5f, 512, 512);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, noiseTex);

}

void NightVision::Render(glm::mat4& view, glm::mat4& projection) 
{
    Pass1(view, projection);
    glFlush();
    Pass2();
}

void NightVision::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}

void NightVision::SetupFBO() 
{
    glGenFramebuffers(1, &renderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

        glGenTextures(1, &renderTex);
        glBindTexture(GL_TEXTURE_2D, renderTex);

            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

            GLuint depthBuf;
            glGenRenderbuffers(1, &depthBuf);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

            GLenum drawbuffers[] = {
                GL_COLOR_ATTACHMENT0
            };

            glDrawBuffers(1, drawbuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NightVision::Pass1(glm::mat4& view, glm::mat4& projection) 
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);

    shader.SetUniform("Light.Position", glm::vec4(0.0f,0.0f,0.0f,1.0f) );
    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.9f, 0.9f));
    shader.SetUniform("Material.Ks", glm::vec3(0.95f, 0.95f, 0.95f));
    shader.SetUniform("Material.Ka", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetUniform("Material.Shininess", 100.0f);

    glm::mat4 model(1.0f);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
    SetMatrices(model, view, projection);
    teapot.RenderModel();

    shader.SetUniform("Material.Kd", glm::vec3(0.4f, 0.4f, 0.4f));
    shader.SetUniform("Material.Ks", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.SetUniform("Material.Ka", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetUniform("Material.Shininess", 1.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f,-0.75f,0.0f));
    SetMatrices(model, view, projection);
    plane.RenderModel();

    shader.SetUniform("Light.Position", glm::vec4(0.0f,0.0f,0.0f,1.0f) );
    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.5f, 0.2f));
    shader.SetUniform("Material.Ks", glm::vec3(0.95f, 0.95f, 0.95f));
    shader.SetUniform("Material.Ka", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetUniform("Material.Shininess", 100.0f);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f,1.0f,3.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));
    SetMatrices(model, view, projection);
    torus.RenderModel();
}

void NightVision::Pass2() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);

    glm::mat4 model(1.0f), view(1.0f), projection(1.0f);  
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    SetMatrices(model, view, projection);
    screenQuad.RenderModel();
}
