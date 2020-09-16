#include "EdgeDetection.h"


Edge::Edge(GLint widht, GLint height) 
{
    this->width = width;
    this->height = height;
}

void Edge::Init() 
{
    shader.CreateFromFiles(
        "./Src/Chapter11/EdgeDetection/shaders/EdgeDetection.vert",
        "./Src/Chapter11/EdgeDetection/shaders/EdgeDetection.frag"
    );

    computeShader.CreateComputeShader("./Src/Chapter11/EdgeDetection/shaders/EdgeDetection.comp");

    screenQuad.LoadModel("./Data/plane.obj");
    plane.LoadModel("./Data/plane.obj");
    teapot.LoadModel("./Data/teapot.obj");
    torus.LoadModel("./Data/Torus.obj");


    SetupFBO();

    shader.UseShader();
    GLuint shaderHandle = shader.GetShaderProgramID();
    pass1Index = glGetSubroutineIndex(shaderHandle, GL_FRAGMENT_SHADER, "Pass1");
    pass2Index = glGetSubroutineIndex(shaderHandle, GL_FRAGMENT_SHADER, "Pass2");

    shader.SetUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    shader.SetUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform("Material.Ka", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.9f, 0.9f));
    shader.SetUniform("Material.Ks", glm::vec3(0.95f, 0.95f, 0.95f));
    shader.SetUniform("Material.Shininess", 100.0f);
}

void Edge::Render(glm::mat4& view, glm::mat4& projection) 
{
    shader.UseShader();
    Pass1(view, projection);

    // computeShader.UseShader();
    // glDispatchCompute(width / 25, height / 25, 1);
    // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // shader.UseShader();
    // Pass2();   
}

void Edge::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));   
}

void Edge::SetupFBO() 
{
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

        GLuint renderTex;
        glGenTextures(1, &renderTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTex);

            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
            glBindImageTexture(0, renderTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

        GLuint edgeTex;
        glGenTextures(1, &edgeTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, edgeTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glBindImageTexture(1, edgeTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

        GLuint depthBuf;
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

        GLenum drawBuffers[] = {
            GL_COLOR_ATTACHMENT0 
        };
        glDrawBuffers(1, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Edge::Pass1(glm::mat4& view, glm::mat4& projection) 
{
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);
    teapot.RenderModel();

    model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
    SetMatrices(model, view, projection);
    plane.RenderModel();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 3.0f));
    SetMatrices(model, view, projection);
    torus.RenderModel();

}

void Edge::Pass2() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);

    glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    SetMatrices(model, view, projection);

    screenQuad.RenderModel(); 
}  
