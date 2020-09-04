#include "TransformFeedback.h"


TransformFeedback::TransformFeedback() 
{
    time = 0.0f;
    deltaTime = 0.0f;

    nParticles = 4000;
    particleLifetime = 6.0f;
    emitterDir = glm::vec3(-1, 2, 0);
    emitterPos = glm::vec3(1, 0, 0);
}

void TransformFeedback::Init() 
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<const char*>* transformFeedbackOutputs = new std::vector<const char*>();
    transformFeedbackOutputs->push_back("Position");
    transformFeedbackOutputs->push_back("Velocity");
    transformFeedbackOutputs->push_back("Age");

    shader.CreateFromFiles(
        "./Src/Chapter10/TransformFeedback/shaders/TransformFeedback.vert",
        "./Src/Chapter10/TransformFeedback/shaders/TransformFeedback.frag",
        "",
        "",
        "",
        transformFeedbackOutputs
    );

    texture.LoadTexture("./Data/Textures/bluewater.png");

    glActiveTexture(GL_TEXTURE1);
    ParticleUtils::CreateRandomTex1D(nParticles * 3);

    InitBuffers();

    shader.UseShader();
    shader.SetUniform("ParticleLifetime", particleLifetime);
    shader.SetUniform("Acceleration", glm::vec3(0.0f, -0.5f, 0.0f));
    shader.SetUniform("ParticleSize", 0.05f);
    shader.SetUniform("EmitterPosition", emitterPos);
    shader.SetUniform("EmitterBasis", ParticleUtils::MakeArbitaryBasis(emitterDir));

}

void TransformFeedback::Update(float deltaTime) 
{
    time += deltaTime;

    this->deltaTime = deltaTime;
}

void TransformFeedback::Render(glm::mat4& view, glm::mat4& projection) 
{
    shader.UseShader();
    texture.UseTexture(GL_TEXTURE0);

    shader.SetUniform("Time", time);
    shader.SetUniform("DeltaTime", deltaTime);

    // Update Pass
    shader.SetUniform("Pass", 1);

    // Disabling Rasterization and preventing going through Fragment Shader
    glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);

        // We enable transform feedback
        // using the function glBeginTransformFeedback and then draw the point primitives.
        glBeginTransformFeedback(GL_POINTS);

            glBindVertexArray(particles[1 - drawBuf]);
                glVertexAttribDivisor(0, 0);
                glVertexAttribDivisor(1, 0);
                glVertexAttribDivisor(2, 0);
                glDrawArrays(GL_POINTS, 0, nParticles);
            glBindVertexArray(0);

        glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    // Render Pass
    shader.SetUniform("Pass", 2);

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);

    glDepthMask(GL_FALSE);
        glBindVertexArray(particles[drawBuf]);
            glVertexAttribDivisor(0, 1);
            glVertexAttribDivisor(1, 1);
            glVertexAttribDivisor(2, 1);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
        glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    // Swapping Buffersnm
    // This is known as ping ponging between Framebuffers
    // One provides the data for current frame, another stores the updated data
    // In next render pass, reverse of this happens
    drawBuf = 1 - drawBuf;
}

void TransformFeedback::InitBuffers() 
{
    // Generating the buffers
    glGenBuffers(2, posBuf);    // Position Buffers
    glGenBuffers(2, velBuf);    // Velocity Buffers
    glGenBuffers(2, ageBuf);    // Age Buffers

    // Allocate space for all Buffers
    int size = nParticles * 3 * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, ageBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, ageBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), 0, GL_DYNAMIC_COPY);

    // Fill the first age buffer
    std::vector<GLfloat> tempData(nParticles);
    float rate = particleLifetime / nParticles;

    for (int i = 0; i < nParticles; i++) {
        tempData[i] = rate * (i - nParticles);
    }

    glBindBuffer(GL_ARRAY_BUFFER, ageBuf[0]);
        // glBufferSubData can be used to update buffer data that is already on gpu
        glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), tempData.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create vertex arrays for each set of buffers
    glGenVertexArrays(2, particles);

    // Setup Particles Array 0
    glBindVertexArray(particles[0]);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

        
        glBindBuffer(GL_ARRAY_BUFFER, ageBuf[0]);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Setup Particles Array 1
    glBindVertexArray(particles[1]);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

        
        glBindBuffer(GL_ARRAY_BUFFER, ageBuf[1]);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Setup Feedback objects
    glGenTransformFeedbacks(2, feedback);

    // Transform feedback 0
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
        // bind a buffer object to an indexed buffer target
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuf[0]);

    // Transform feedback 1
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuf[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

}

void TransformFeedback::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}
