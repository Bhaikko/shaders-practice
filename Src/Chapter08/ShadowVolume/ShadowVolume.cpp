#include "ShadowVolume.h"

ShadowVolume::ShadowVolume(GLint width, GLint height) 
{
    this->width = width;
    this->height = height;

    angle = 0.0f;
    rotSpeed = 15.0f;

    lightPos = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void ShadowVolume::Init() 
{
    volumeShader.CreateFromFiles(
        "./Src/Chapter08/ShadowVolume/shaders/ShadowVolume.vert",
        "./Src/Chapter08/ShadowVolume/shaders/ShadowVolume.frag",
        "./Src/Chapter08/ShadowVolume/shaders/ShadowVolume.geom"
    );

    renderShader.CreateFromFiles(
        "./Src/Chapter08/ShadowVolume/shaders/ShadowVolume-render.vert",
        "./Src/Chapter08/ShadowVolume/shaders/ShadowVolume-render.frag"
    );

    compShader.CreateFromFiles(
        "./Src/Chapter08/ShadowVolume/shaders/ShadowVolume-comp.vert",
        "./Src/Chapter08/ShadowVolume/shaders/ShadowVolume-comp.frag"
    );

    screenQuad.LoadModel("./Data/plane.obj");
    plane.LoadModel("./Data/plane.obj");
    bunny.LoadModel("./Data/teapot.obj");

    glClearStencil(0);

    SetupFBO();

    renderShader.UseShader();
    renderShader.SetUniform("LightIntensity", glm::vec3(1.0f));

    brickTexture.LoadTexture("./Data/Textures/brick.png");
    brickTexture.UseTexture(GL_TEXTURE2);

    mossTexture.LoadTexture("./Data/Textures/moss.png");

    UpdateLight();

    renderShader.UseShader();
    renderShader.SetUniform("Tex", 2);

    compShader.UseShader();
    compShader.SetUniform("DiffSpecTex", 0);
}

void ShadowVolume::Render(glm::mat4& view, glm::mat4& projection) 
{
    Pass1(view, projection);
        glFlush();
    Pass2(view, projection);
        glFlush();
    Pass3();
}

void ShadowVolume::Update(float deltaTime) 
{
    angle += rotSpeed * deltaTime;
    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }

    UpdateLight();
}

void ShadowVolume::SetMatrices(Shader& shader, glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelViewMatrix", mv);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}

void ShadowVolume::SetupFBO() 
{   
    // Creating Depth Buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // Ambient Buffer
    GLuint ambBuf;
    glGenRenderbuffers(1, &ambBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, ambBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);

    // Diffuse Specular Component
    glActiveTexture(GL_TEXTURE0);
    GLuint diffSpecTex;
    glGenTextures(1, &diffSpecTex);
    glBindTexture(GL_TEXTURE_2D, diffSpecTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenFramebuffers(1, &colorDepthFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ambBuf);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffSpecTex, 0);

        GLenum drawBuffers[] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1 
        };
        glDrawBuffers(2, drawBuffers);

        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result == GL_FRAMEBUFFER_COMPLETE) {
            printf("Framebuffer Complete.\n");
        } else {
            printf("Framebuffer Incomplete.\n");
        }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);    


}

void ShadowVolume::DrawScene(Shader& shader, glm::mat4& view, glm::mat4& projection, bool onlyShadowCasters) 
{
    glm::vec3 color(1.0f);
    
    if (!onlyShadowCasters) {
        brickTexture.UseTexture(GL_TEXTURE2);
        shader.SetUniform("Ka", color * 0.1f);
        shader.SetUniform("Kd", color);
        shader.SetUniform("Ks", glm::vec3(0.9f));
        shader.SetUniform("Shininess", 150.0f);
    }

    glm::mat4 model(1.0f);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.3f, 1.0f, 0.2f));
    SetMatrices(shader, model, view, projection);
    bunny.RenderAsAdjacency();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.5f, 1.0f, -1.2f));
    SetMatrices(shader, model, view, projection);
    bunny.RenderAsAdjacency();
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, 1.0f, 2.7f));
    SetMatrices(shader, model, view, projection);
    bunny.RenderAsAdjacency();

    if (!onlyShadowCasters) {    
        brickTexture.UseTexture(GL_TEXTURE2);
        color = glm::vec3(0.5f);
        
        shader.SetUniform("Kd", color);
        shader.SetUniform("Ks", glm::vec3(0.0f));
        shader.SetUniform("Ka", glm::vec3(0.1f));
        shader.SetUniform("Shininess", 1.0f);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
        SetMatrices(shader, model, view, projection);
        plane.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f,5.0f,0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f,0.0f,1.0f));
        model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
        SetMatrices(shader, model, view, projection);
        plane.RenderModel();    

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,5.0f,-5.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
        SetMatrices(shader, model, view, projection);
        plane.RenderModel();
    }

}

// Rending geometry normally with shading
// THe ambient component is rendered to one buffer,
// The diffuse and specular components are written to texture
void ShadowVolume::Pass1(glm::mat4& view, glm::mat4& projection) 
{
    // Enabling writing to depth buffer
    glDepthMask(GL_TRUE);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);

    renderShader.UseShader();
    renderShader.SetUniform("LightPosition", view * lightPos);

    glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawScene(renderShader, view, projection, false);
}

// Generating Shadow Volumes using Geometry shaders
void ShadowVolume::Pass2(glm::mat4& view, glm::mat4& projection) 
{
    volumeShader.UseShader();
    volumeShader.SetUniform("LightPosition", view * lightPos);

    // Copying depth and color bufferes from colorFBO to defaultFBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER, colorDepthFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // copy a block of pixels from one framebuffer object to another
    glBlitFramebuffer(0, 0, width - 1, height - 1, 0, 0, width - 1, height - 1, GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Disable writing to color buffer and depth buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup stencil test so that it always succeeds, 
    // Increments for front faces and decrements for back faces
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, 0xffff);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);


    // Draw only the shadow casters
    DrawScene(volumeShader, view, projection, true);

    // Enabling writing to color mask
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

// Reading the diffuse and specular component from texture 
// and combine it with the amvient componet if stencil test succeeds
void ShadowVolume::Pass3() 
{
    // We dont need the depth test
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    // Only rendering pixels that have stencil value equal to zero
    glStencilFunc(GL_EQUAL, 0, 0xffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    compShader.UseShader();

    glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    SetMatrices(compShader, model, view, projection);

    // mossTexture.UseTexture(GL_TEXTURE0);

    screenQuad.RenderModel();

    // Restore State for Pass 1
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void ShadowVolume::UpdateLight() 
{
    lightPos = glm::vec4(5.0f * glm::vec3(cosf(angle) * 7.5f, 1.5f, sinf(angle) * 7.5f), 1.0f);
}
