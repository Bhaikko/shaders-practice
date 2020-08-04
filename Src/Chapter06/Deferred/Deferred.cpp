#include "Deferred.h"

Deferred::Deferred(GLint& width, GLint& height)
{
    this->width = width;
    this->height = height;
}

void Deferred::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter06/Deferred/shaders/Deferred.vert",
        "./Src/Chapter06/Deferred/shaders/Deferred.frag"
    );

    screenQuad.LoadModel("./Data/plane.obj");
    plane.LoadModel("./Data/plane.obj");
    torus.LoadModel("./Data/Torus.obj");
    teapot.LoadModel("./Data/teapot.obj");

    SetupFbo();

    shader.UseShader();

    shader.SetUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    shader.SetUniform("Light.L", glm::vec3(1.0f));
}

void Deferred::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();
    Pass1(view, projection);
    Pass2();
}


void Deferred::SetMatrices(glm::mat4 model, glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(glm::inverseTranspose(view * model)));
}

void Deferred::SetupFbo()
{
    GLuint depthBuf, posTex, normTex, colorTex;

    glGenFramebuffers(1, &deferredFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);

        // The depth buffer
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

        // Create textures for position, normal and color
        CreateGBufferTex(GL_TEXTURE0, GL_RGBA32F, posTex);
        CreateGBufferTex(GL_TEXTURE1, GL_RGBA32F, normTex);
        CreateGBufferTex(GL_TEXTURE2, GL_RGBA8, colorTex);

        // Attach the textures to framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

        // To select which buffer(s) you want to render to, you use glDrawBuffers()
        // To produce output for multiple color buffers, you define multiple outputs in the fragment shader.
        GLenum drawBuffers[] = {
            GL_NONE,                // Since in fragment shader, FragColor at location 0 isnt required
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2
        };

        glDrawBuffers(4, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Deferred::CreateGBufferTex(GLenum textureUnit, GLenum format, GLuint& textureID)
{
    glActiveTexture(textureUnit);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

}

void Deferred::Pass1(glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("Pass", 1);

    glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model(1.0f);
    
    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.9f, 0.9f));

    SetMatrices(model, view, projection);
    teapot.RenderModel();


    shader.SetUniform("Material.Kd", glm::vec3(0.4f, 0.4f, 0.4f));
    model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
    SetMatrices(model, view, projection);
    plane.RenderModel();


    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.5f, 0.2f));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.5f, 0.2f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    SetMatrices(model, view, projection);

    torus.RenderModel();

    glFinish();

}

void Deferred::Pass2()
{
    shader.SetUniform("Pass", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    SetMatrices(model, view, projection);

    screenQuad.RenderModel();
}
