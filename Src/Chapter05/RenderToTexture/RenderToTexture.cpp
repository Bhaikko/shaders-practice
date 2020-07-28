#include "RenderToTexture.h"

RenderToTexture::RenderToTexture()
{
    angle = 0.0f;
    rotSpeed = 15.0f;
}

void RenderToTexture::Init()
{
    texture.LoadTexture("./Data/Textures/flower.png");

    cow.LoadModel("./Data/cow.obj");
    cube.LoadModel("./Data/cube.obj");
    shader.CreateFromFiles(
        "./Src/Chapter05/RenderToTexture/shaders/RenderToTexture.vert",
        "./Src/Chapter05/RenderToTexture/shaders/RenderToTexture.frag"
    );

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.L");
    shader.RegisterUniform("Light.La");

    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");   

    shader.RegisterUniform("RenderTex");

    shader.UseShader();

    glUniform3f(shader.GetUniformLocation("Light.L"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.15f, 0.15f, 0.15f);

    SetupFBO();

}

void RenderToTexture::SetupFBO()
{
    // Generate and bind framebuffer
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

        // Create Texture object
        GLuint renderTex;
        glGenTextures(1, &renderTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTex);
        
        /* 
        glTexStorage2D specify the storage requirements for all levels 
        of a two-dimensional texture or one-dimensional texture array simultaneously. 
        Once a texture is specified with this command, 
        the format and dimensions of all levels become immutable unless it is a proxy texture. The contents of the image may still be modified, 
        however, its storage requirements may not change. Such a texture is referred to as an immutable-format texture
        */
        // Allocating storage to Texture Buffer object
        // glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, 512, 512);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Bind Texture to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

        // Since depth value is calculated by Fragment shader which will write
        // Depth in render buffer, hence we need a render buffer and
        // this data is later attached to Framebuffer

        // Create Depth Buffer
        GLuint depthBuf;
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        // establishing data storage, format and dimensions of a renderbuffer object's image
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

        // attach a renderbuffer object to a framebuffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

        
        // Set the targets for fragment output variables
        // Specifies a list of color buffers to be drawn into
        GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, &drawBuffer);

        /* 
        Such as with the read buffer, framebuffers have a set of draw buffers where writing operations write to. 
        Unlike the read buffer, there are multiple draw buffers, 
        which allow writing operations to write values to different buffers in the framebuffer at the same time.
        */

       GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
       if (result == GL_FRAMEBUFFER_COMPLETE) {
           std::cout << "Framebuffer is complete" << std::endl;
       } else {
           std::cout << "Framebuffer error: " << result << std::endl;
       }

    // Unbind and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderToTexture::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    RenderTexture();
    glFlush();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RenderScene(view, projection);
}

void RenderToTexture::RenderTexture()
{
    glUniform1i(shader.GetUniformLocation("RenderTex"), 1);
    texture.UseTexture(GL_TEXTURE1);
    glViewport(0, 0, 512, 512);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 projection = glm::perspective(glm::radians(50.0f), 1.0f, 0.3f, 100.0f);

    glUniform4f(shader.GetUniformLocation("Light.Position"), 0.0f, 0.0f, 0.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.95f, 0.95f, 0.95f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 100.0f);

    glm::mat4 model(1.0f);
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    cow.RenderModel();
}

void RenderToTexture::RenderScene(glm::mat4 view, glm::mat4 projection)
{
    glUniform1i(shader.GetUniformLocation("RenderTex"), 0);
    glViewport(0, 0, 800, 600);
    

    glUniform4f(shader.GetUniformLocation("Light.Position"), 0.0f, 0.0f, 0.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.0f, 0.0, 0.0);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 1.0f);

    glm::mat4 model(1.0f);

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    cube.RenderModel();
}

void RenderToTexture::Update(float deltaTime)
{
    angle += rotSpeed * deltaTime;
    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }
}   
