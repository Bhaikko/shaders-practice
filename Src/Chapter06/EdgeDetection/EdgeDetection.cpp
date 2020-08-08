#include "EdgeDetection.h"

EdgeDetection::EdgeDetection(GLint width, GLint height, bool bApplyEdgeDetection)
{
    this->width = width;
    this->height = height;
    this->bApplyEdgeDetection = bApplyEdgeDetection;
}

void EdgeDetection::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter06/EdgeDetection/shaders/EdgeDetection.vert",
        "./Src/Chapter06/EdgeDetection/shaders/EdgeDetection.frag"
    );

    torus.LoadModel("./Data/Torus.obj");
    plane.LoadModel("./Data/bunny.obj");
    teapot.LoadModel("./Data/teapot.obj");
    screenQuad.LoadModel("./Data/plane.obj");

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

    shader.RegisterUniform("EdgeThreshold");
    shader.RegisterUniform("Pass");

    shader.UseShader();

    glUniform1f(shader.GetUniformLocation("EdgeThreshold"), 0.05f);
    glUniform4f(shader.GetUniformLocation("Light.Position"), 0.0f, 0.0f, 0.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.L"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);
}

void EdgeDetection::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    Pass1(view, projection);

    if (bApplyEdgeDetection) {
        Pass2();
    }
}

void EdgeDetection::SetupFBO()
{
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

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
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

                GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
                glDrawBuffers(1, &drawBuffer);
            
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void EdgeDetection::Pass1(glm::mat4 view, glm::mat4 projection)
{
    glUniform1i(shader.GetUniformLocation("Pass"), 1);

    if (bApplyEdgeDetection) {
        glBindFramebuffer(GL_FRAMEBUFFER, fboHandle); 
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));


    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.9f, 0.9f, 0.9f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.95f, 0.95f, 0.95f);
    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.1f, 0.1f, 0.1f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 100.0f);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    teapot.RenderModel();

    glUniform3f(shader.GetUniformLocation("Material.Kd"), 0.4f, 0.4f, 0.4f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.0f, 0.0f, 0.0f);
    glUniform3f(shader.GetUniformLocation("Material.Ka"), 0.1f, 0.1f, 0.1f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 1.0f);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    plane.RenderModel();

}

void EdgeDetection::Pass2()
{
    glUniform1i(shader.GetUniformLocation("Pass"), 2);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glDisable(GL_DEPTH_TEST);

    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    screenQuad.RenderModel();

}
