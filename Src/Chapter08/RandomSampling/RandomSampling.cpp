#include "RandomSampling.h"

RandomSampling::RandomSampling(GLint width, GLint height)
{
    this->width = width;
    this->height = height;

    shadowMapWidth = 1000;
    shadowMapHeight = 1000;

    samplesU = 4;
    samplesV = 8;
    jitterMapSize = 8;
    radius = 7.0f;
}

void RandomSampling::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter08/RandomSampling/shaders/RandomSampling.vert",
        "./Src/Chapter08/RandomSampling/shaders/RandomSampling.frag"
    );

    plane.LoadModel("./Data/plane.obj");
    building.LoadModel("./Data/building.obj");


    SetupFBO();
    BuildJitterTex();

    GLuint programHandle = shader.GetShaderProgramID();
    pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "RecordDepth");
    pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "ShadeWithShadow");

    shadowScale = glm::mat4(
        glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
    );

    lightPos = glm::vec3(-2.5f, 2.0f, -2.5f);
    lightFrustum.Orient(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightFrustum.SetPerspective(40.0f, 1.0f, 0.1f, 100.0f);

    lightPOV = shadowScale * lightFrustum.GetProjectionMatrix() * lightFrustum.GetViewMatrix();

    shader.UseShader();

    shader.SetUniform("Light.Intensity", glm::vec3(0.85f));

    shader.SetUniform("ShadowMap", 0);
    shader.SetUniform("OffsetTex", 1);
    shader.SetUniform("Radius", radius / 512.0f);
    shader.SetUniform("OffsetTexSize", glm::vec3(jitterMapSize, jitterMapSize, samplesU * samplesV / 2.0f));

}

void RandomSampling::Render(glm::mat4& view, glm::mat4& projection)
{
    // Pass1 (shadow Map generation)
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);
        glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(2.5f, 10.0f);

                glm::mat4 lightView = lightFrustum.GetViewMatrix();
                glm::mat4 lightProjection = lightFrustum.GetProjectionMatrix();
                DrawBuildingScene(lightView, lightProjection);
            glDisable(GL_POLYGON_OFFSET_FILL);    

    shader.SetUniform("Light.Position", view * glm::vec4(lightFrustum.GetOrigin(), 1.0f));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
        glDisable(GL_CULL_FACE);

        DrawBuildingScene(view, projection);

    glFinish();
}

void RandomSampling::SetupFBO()
{
    GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };

    GLuint depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, shadowMapWidth, shadowMapHeight);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

        GLenum drawBuffers[] = {
            GL_NONE 
        };

        glDrawBuffers(1, drawBuffers);

        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result == GL_FRAMEBUFFER_COMPLETE) {
            printf("Framebuffer is complete. \n");
        } else {
            printf("Framebuffer error. \n");
        }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
}

float RandomSampling::Jitter()
{
    static std::default_random_engine generator;
    static std::uniform_real_distribution<float> distrib(-0.5f, 0.5f);

    return distrib(generator);
}


void RandomSampling::BuildJitterTex()
{
    int size = jitterMapSize;
    int samples = samplesU * samplesV;

    int bufSize = size * size * samples * 2;
    float* data = new float[bufSize];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < samples; k += 2) {
                int x1, y1, x2, y2;
                x1 = k % samplesU;
                y1 = (samples - 1 - k) / samplesU;

                x2 = (k + 1) % samplesU;
                y2 = (samples - 1 - k - 1) / samplesU;

                glm::vec4 v;

                // Center on grid and jitter
                v.x = (x1 + 0.5f) + Jitter();
                v.y = (y1 + 0.5f) + Jitter();
                v.z = (x2 + 0.5f) + Jitter();
                v.w = (y2 + 0.5f) + Jitter();

                // Scale between 0 and 1
                v.x /= samplesU;
                v.y /= samplesV;
                v.z /= samplesU;
                v.w /= samplesV;

                // Wrap to disk
                int cell = ((k / 2) * size * size + j * size + i) * 4;
                data[cell + 0] = sqrtf(v.y) * cosf(glm::two_pi<float>() * v.x);
                data[cell + 1] = sqrtf(v.y) * sinf(glm::two_pi<float>() * v.x);
                data[cell + 2] = sqrtf(v.w) * cosf(glm::two_pi<float>() * v.z);
                data[cell + 3] = sqrtf(v.w) * sinf(glm::two_pi<float>() * v.z);
            }
        }
    }

    glActiveTexture(GL_TEXTURE1);
    GLuint texID;
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA32F, size, size, samples / 2);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, size, size, samples / 2, GL_RGBA, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    delete[] data;
}

void RandomSampling::DrawBuildingScene(glm::mat4& view, glm::mat4& projection)
{
    glm::vec3 color = glm::vec3(1.0f, 0.85f, 0.55f);
    shader.SetUniform("Material.Ka", color * 0.1f);
    shader.SetUniform("Material.Kd", color);
    shader.SetUniform("Material.Ks", glm::vec3(0.0f));
    shader.SetUniform("Material.Shininess", 1.0f);

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);
    building.RenderModel();

    plane.RenderModel();

}

void RandomSampling::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
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
    shader.SetUniform("ShadowMatrix", lightPOV * model);
}