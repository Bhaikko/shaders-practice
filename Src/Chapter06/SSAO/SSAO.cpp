#include "SSAO.h"

SSAO::SSAO(GLint width, GLint height)
{
    this->width = width;
    this->height = height;
}

void SSAO::Init()
{
    dragon.LoadModel("./Data/bunny.obj");
    plane.LoadModel("./Data/plane.obj");
    
    screenFillQuad.LoadModel("./Data/plane.obj");

    brickTex.LoadTexture("./Data/Textures/mybrick.png");
    woodTex.LoadTexture("./Data/Textures/dirt.png");

    shader.CreateFromFiles(
        "./Src/Chapter06/SSAO/shaders/SSAO.vert",
        "./Src/Chapter06/SSAO/shaders/SSAO.frag"
    );

    shader.UseShader();

    shader.SetUniform("Light.Position", glm::vec4(3.0f, 3.0f, 1.5f, 1.0f));
    shader.SetUniform("Light.L", glm::vec3(0.3f));
    shader.SetUniform("Light.La", glm::vec3(0.5f));

    // Create and assign random sample kernel
    BuildKernel();

    // Create texture of random rotation directions
    GLuint rotTex = BuildRanRotationTex();
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, rotTex);

    SetupFBO();
}

void SSAO::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();
    Pass1(view, projection);    // Render to G-buffers
    Pass2();    // SSAO
    Pass3();    // Blur 
    Pass4();    // Lighting
}


GLuint SSAO::BuildRanRotationTex()
{
    int size = 4;
    std::vector<GLfloat> randDirections(3 * size * size);

    for (int i = 0; i < size * size; i++) {
        glm::vec3 v = rand.uniformCircle();
        randDirections[i * 3 + 0] = v.x;
        randDirections[i * 3 + 1] = v.y;
        randDirections[i * 3 + 2] = v.z;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, size, size);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGB, GL_FLOAT, randDirections.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    return tex;

}

void SSAO::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(glm::inverseTranspose(view * model)));
}

void SSAO::SetupFBO()
{
    GLuint depthBuf, posTex, normTex, colorTex;

    CreateGBuffTex(GL_TEXTURE0, GL_RGBA32F, posTex);
    CreateGBuffTex(GL_TEXTURE1, GL_RGBA32F, normTex);
    CreateGBuffTex(GL_TEXTURE2, GL_RGBA8, colorTex);

    CreateGBuffTex(GL_TEXTURE3, GL_R16F, aoTex[0]);
    CreateGBuffTex(GL_TEXTURE3, GL_R16F, aoTex[1]);

    glGenFramebuffers(1, &deferredFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);

        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

        GLenum drawBuffers[] = {
            GL_NONE,
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_NONE 
        };

        glDrawBuffers(5, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aoTex[0], 0);

    GLenum drawBuffer2[] = {
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_COLOR_ATTACHMENT0
    };

    glDrawBuffers(5, drawBuffer2);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::CreateGBuffTex(GLenum texUnit, GLenum format, GLuint& texID)
{
    glActiveTexture(texUnit);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

void SSAO::BuildKernel()
{
    int kernSize = 64;
    std::vector<float> kern(3 * kernSize);

    for (int i = 0; i < kernSize; i++) {
        glm::vec3 randDir = rand.uniformHemisphere();

        float scale = ((float)(i * i)) / (kernSize * kernSize);
        randDir *= glm::mix(0.1f, 1.0f, scale);

        kern[i * 3 + 0] = randDir.x;
        kern[i * 3 + 1] = randDir.y;
        kern[i * 3 + 2] = randDir.z;
    }

    glUniform3fv(shader.GetUniformLocation("SampleKernel"), kernSize, kern.data());

}


void SSAO::Pass1(glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("Pass", 1);

    glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    DrawScene(view, projection);
}

void SSAO::Pass2()
{
    shader.SetUniform("Pass", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aoTex[0], 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    DrawScreenFillingQuad();
}

void SSAO::Pass3()
{
    shader.SetUniform("Pass", 3);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, aoTex[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aoTex[1], 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    DrawScreenFillingQuad();
}

void SSAO::Pass4()
{
    shader.SetUniform("Pass", 4);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, aoTex[1]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    DrawScreenFillingQuad();
}


void SSAO::DrawScreenFillingQuad()
{
    glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    SetMatrices(model, view, projection);

    screenFillQuad.RenderModel();
}

void SSAO::DrawScene(glm::mat4& view, glm::mat4& projection)
{
    // Walls
    woodTex.UseTexture(GL_TEXTURE5);
    shader.SetUniform("Material.UseTex", 1);
    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);
    plane.RenderModel();

    brickTex.UseTexture(GL_TEXTURE5);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    SetMatrices(model, view, projection);
    plane.RenderModel();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	SetMatrices(model, view, projection);
	plane.RenderModel();

	shader.SetUniform("Material.UseTex", 0);
	shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.5f, 0.2f));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0, 0.282958, 0));
    SetMatrices(model, view, projection);
    dragon.RenderModel();

}
