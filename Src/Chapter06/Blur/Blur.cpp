#include "Blur.h"

Blur::Blur(GLint width, GLint height)
{
    this->width = width;
    this->height = height;
}

void Blur::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter06/Blur/shaders/Blur.vert",
        "./Src/Chapter06/Blur/shaders/Blur.frag"
    );

    screenQuad.LoadModel("./Data/plane.obj");
    teapot.LoadModel("./Data/teapot.obj");
    torus.LoadModel("./Data/Torus.obj");
    cube.LoadModel("./Data/cube.obj");

    SetupFBO();

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.L");
    shader.RegisterUniform("Light.La");

    shader.RegisterUniform("Material.Ka");
    shader.RegisterUniform("Material.Kd");
    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.RegisterUniform("Weights[0]");
    shader.RegisterUniform("Weights[1]");
    shader.RegisterUniform("Weights[2]");
    shader.RegisterUniform("Weights[3]");
    shader.RegisterUniform("Weights[4]");

    shader.RegisterUniform("Pass");
    shader.RegisterUniform("RenderTexture");

    shader.UseShader();

    glUniform4f(shader.GetUniformLocation("Light.Position"), 0.0f, 0.0f, 0.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.L"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);

    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.9f, 0.9f, 0.9f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.95f, 0.95f, 0.95f);
    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.1f, 0.1f, 0.1f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 100.0f);
    

    // Weights Initilisation
    float weights[5], sum, sigma2 = 8.0f;

    weights[0] = Guass(0, sigma2);
    sum = weights[0];
    for (int i = 1; i < 5; i++) {
        weights[i] = Guass(float(i), sigma2);
        sum += 2 * weights[i];
    }


    for (unsigned int i = 0; i < 5; i++) {
        std::string uniformName = "Weights[" + std::to_string(i) + "]";
        float value = weights[i] / sum;
        glUniform1f(shader.GetUniformLocation(uniformName), value);
    }
}

void Blur::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    Pass1(view, projection);
    Pass2();
    Pass3();
}

void Blur::SetupFBO()
{
    // Setting up Render Framebuffer
    glGenFramebuffers(1, &renderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

        glGenTextures(1, &renderTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTex);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

            GLuint depthBuf;
            glGenRenderbuffers(1, &depthBuf);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

                GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
                glDrawBuffers(1, &drawBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // Setting up Intermediate Framebuffer
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

        glGenTextures(1, &intermediateTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, intermediateTex);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTex, 0);

            glDrawBuffers(1, &drawBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void Blur::Pass1(glm::mat4 view, glm::mat4 projection)
{
    glUniform1i(shader.GetUniformLocation("Pass"), 1);

    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 0.0f));
    SetupMatrices(model, view, projection);
    teapot.RenderModel();

    model = glm::mat4(1.0f);
    SetupMatrices(model, view, projection);
    torus.RenderModel();

}

void Blur::Pass2()
{
    glUniform1i(shader.GetUniformLocation("Pass"), 2);

    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    SetupMatrices(model, view, projection);
    screenQuad.RenderModel();
}

void Blur::Pass3()
{
    glUniform1i(shader.GetUniformLocation("Pass"), 3);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, intermediateTex);

    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    SetupMatrices(model, view, projection);
    screenQuad.RenderModel();
}

float Blur::Guass(float x, float sigma2)
{
    double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
    double exponenet = -(x * x) / (2.0 * sigma2);

    return (float)(coeff * exp(exponenet));
}


void Blur::SetupMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));
}