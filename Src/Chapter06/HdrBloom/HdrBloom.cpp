#include "HdrBloom.h"

HdrBloom::HdrBloom(GLint width, GLint height)
{
    bloomBufWidth = 0;
    bloomBufHeight = 0;

    this->height = height;
    this->width = width;
}

void HdrBloom::Init()
{
    screenQuad.LoadModel("./Data/plane.obj");
    plane.LoadModel("./Data/plane.obj");
    teapot.LoadModel("./Data/teapot.obj");
    sphere.LoadModel("./Data/sphere.obj");

    shader.CreateFromFiles(
        "./Src/Chapter06/HdrBloom/shaders/HdrBloom.vert",
        "./Src/Chapter06/HdrBloom/shaders/HdrBloom.frag"
    );

    SetupFBO();

    shader.UseShader();

    glm::vec3 intense = glm::vec3(0.6f);
    shader.SetUniform("Lights[0].L", intense);
    shader.SetUniform("Lights[1].L", intense);
    shader.SetUniform("Lights[2].L", intense);

    intense = glm::vec3(0.2f);
    shader.SetUniform("Lights[0].La", intense);
    shader.SetUniform("Lights[1].La", intense);
    shader.SetUniform("Lights[2].La", intense);

    
    shader.SetUniform("LumThresh", 1.0f);

    float weights[10], sum, sigma2 = 25.0f;

    weights[0] = Guass(0, sigma2);
    sum = weights[0];
    for (unsigned int i = 1; i < 10; i++) {
        weights[i] = Guass(float(i), sigma2);
        sum += 2 * weights[i];
    }

    // Normalising the weights and set the uniform
    for (unsigned int i = 0; i < 10; i++) {
        std::string uniformName = "Weights[" + std::to_string(i) + "]";
        float value = weights[i] / sum;
        shader.SetUniform(uniformName, value);
    }

    // Setup two sampler objects for linear and nearest filtering
    GLuint samplers[2];
    glGenSamplers(2, samplers);
    linearSampler = samplers[0];
    nearestSampler = samplers[1];

    GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Set up the nearest sampler
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(nearestSampler, GL_TEXTURE_BORDER_COLOR, border);

    // Setup the linear sampler
    glSamplerParameteri(linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(linearSampler, GL_TEXTURE_BORDER_COLOR, border);

    glBindSampler(0, nearestSampler);
    glBindSampler(1, nearestSampler);
    glBindSampler(2, nearestSampler);

}


void HdrBloom::SetMatrics(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(glm::inverseTranspose(view * model)));
}

void HdrBloom::SetupFBO()
{
    glGenFramebuffers(1, &hdrFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);

        glGenTextures(1, &hdrTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTex);

        // The internal format must be sized, hence use GL_RGBA<size>
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);

        //The format (7th argument), together with the type argument, describes the data you pass in as the last argument. 
        // So the format/type combination defines the memory layout of the data you pass in.

        // internalFormat (2nd argument) defines the format that OpenGL should use to store the data internally.
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTex, 0);

            GLuint depthBuf;
            glGenRenderbuffers(1, &depthBuf);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

                GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
                glDrawBuffers(1, &drawBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    bloomBufWidth = width / 8;
    bloomBufHeight = height / 8;

    glGenFramebuffers(1, &blurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);



        // Creating 2 texture objects to ping pong for bright pass filter
        // and the two pass blur
        glGenTextures(1, &tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, bloomBufWidth, bloomBufHeight);

        glGenTextures(1, &tex2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, bloomBufWidth, bloomBufHeight);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

            glDrawBuffers(1, &drawBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

float HdrBloom::Guass(float x, float sigma2)
{
    double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
    double expon = -(x * x) / (2.0 * sigma2);
    return (float)(coeff * exp(expon));
}

void HdrBloom::ComputeLogAveLumninace()
{
    GLuint size = width * height;
    std::vector<GLfloat> texData(size * 3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTex);

    // return a texture image into pixels.
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData.data());

    float sum = 0.0f;

    // Formula in book to calculate this log-average 
    // then used as part of the tone mapping operator
    for (unsigned int i = 0; i < size; i++) {
        float luminance = glm::dot(
            glm::vec3(texData[i * 3 + 0], texData[i * 3 + 1], texData[i * 3 + 2]),
            glm::vec3(0.2126f, 0.7152f, 0.0722f) 
        );

        sum += logf(luminance + 0.00001f);
    }

    shader.SetUniform("AveLum", expf(sum / size));
}


void HdrBloom::Pass1(glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("Pass", 1);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    DrawScene(view, projection);
}

void HdrBloom::Pass2()
{
    shader.SetUniform("Pass", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);

    // Rendering to tex1
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    glViewport(0, 0, bloomBufWidth, bloomBufHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    SetMatrics(model, view, projection);


    screenQuad.RenderModel();
}

void HdrBloom::Pass3()
{
    shader.SetUniform("Pass", 3);


    // Rending to tex2
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);

    screenQuad.RenderModel();
}

void HdrBloom::Pass4()
{
    shader.SetUniform("Pass", 4);

    // Rendering to tex1
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    screenQuad.RenderModel();
}

void HdrBloom::Pass5()
{
    shader.SetUniform("Pass", 5);

    // Rendering to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);

    // Linear sampling for extra blur
    glBindSampler(1, linearSampler);

    screenQuad.RenderModel();

    // Reverting back to nearest Sampling
    glBindSampler(1, nearestSampler);
}


void HdrBloom::DrawScene(glm::mat4& view, glm::mat4& projection)
{
    glm::vec4 lightPos = glm::vec4(0.0f, 4.0f, 2.5f, 1.0f);
    
    lightPos.x =  -7.0f;
    shader.SetUniform("Lights[0].Position", view * lightPos);

    lightPos.x =  0.0f;
    shader.SetUniform("Lights[1].Position", view * lightPos);

    lightPos.x =  7.0f;
    shader.SetUniform("Lights[2].Position", view * lightPos);

    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.3f, 0.2f));
    shader.SetUniform("Material.Ks", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform("Material.Ka", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.SetUniform("Material.Shininess", 25.0f);

    glm::mat4 model(1.0f);
    SetMatrics(model, view, projection);
    
    // backdrop plane
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.8f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
    shader.SetUniform("ModelMatrix", model);
    plane.RenderModel();

    // Bottom Plane
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
    shader.SetUniform("ModelMatrix", model);
    plane.RenderModel();

    // Top plane
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
    shader.SetUniform("ModelMatrix", model);
    plane.RenderModel();

    // Sphere
    shader.SetUniform("Material.Kd", glm::vec3(0.4f, 0.9f, 0.4f));
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
    shader.SetUniform("ModelMatrix", model);
    sphere.RenderModel();

    shader.SetUniform("Material.Kd", glm::vec3(0.4f, 0.4f, 0.9f));
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f,0.0f, 0.0f));
    shader.SetUniform("ModelMatrix", model);
    teapot.RenderModel();

}


void HdrBloom::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();

    Pass1(view, projection);
    ComputeLogAveLumninace();
    Pass2();
    Pass3();
    Pass4();
    Pass5();
}

