#include "SamplerObject.h"

SamplerObject::SamplerObject()
{
    angle = 0.0f;
}

void SamplerObject::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter05/SamplerObject/shaders/SamplerObject.vert",
        "./Src/Chapter05/SamplerObject/shaders/SamplerObject.frag"
    );

    plane.LoadModel("./Data/plane.obj");

    CreateTexture();

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("Light.Position");
    shader.RegisterUniform("Light.L");
    shader.RegisterUniform("Light.La");

    shader.RegisterUniform("Material.Ks");
    shader.RegisterUniform("Material.Shininess");

    shader.UseShader();

    glUniform4f(shader.GetUniformLocation("Light.Position"), 0.0f, 20.0f, 0.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.L"), 1.0f, 1.0f, 1.0f);
    glUniform3f(shader.GetUniformLocation("Light.La"), 0.2f, 0.2f, 0.2f);

    glUniform3f(shader.GetUniformLocation("Material.Ks"), 0.95f, 0.95f, 0.95f);
    glUniform1f(shader.GetUniformLocation("Material.Shininess"), 100.0f);

    

}

void SamplerObject::CreateTexture()
{
    GLuint width = 128, height = 128, checkSize = 4;

    GLubyte* data = new GLubyte[width * height * 4];
    for (unsigned int r = 0; r < height; r++) {
        for (unsigned int c = 0; c < width; c++) {
            GLubyte color = 0;
            if (((c / checkSize) + (r / checkSize)) % 2 == 0) {
                color = 0;
            } else {
                color = 255;
            }

            data[(r * width + c) * 4 + 0] = color;
            data[(r * width + c) * 4 + 1] = color;
            data[(r * width + c) * 4 + 2] = color;
            data[(r * width + c) * 4 + 3] = 255;
        }
    }

    // Create Texture object
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        // Texturing maps a portion of a specified texture image onto each graphical primitive for which texturing is enabled.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Creating sampler objects
        GLuint samplers[2];
        glGenSamplers(2, samplers);
        linearSampler = samplers[0];
        nearestSampler = samplers[1];

        // Setup nearest sampler
        glSamplerParameteri(nearestSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(nearestSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Setup linear sampler
        glSamplerParameteri(linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


}

void SamplerObject::Update(float deltaTime)
{

}

void SamplerObject::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(-1.01f, 0.0f, 0.0f));
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));
    
    // Binding sampler to texture unit
    glBindSampler(0, nearestSampler);
    plane.RenderModel();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.01f, 0.0f, 0.0f));
    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    glBindSampler(0, linearSampler);
    plane.RenderModel();

}
