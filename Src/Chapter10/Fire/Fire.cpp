#include "Fire.h"

Fire::Fire() 
{
    time = 0.0f;
    deltaTime = 0.0f;

    nParticles = 2000;
    particleSize = 0.5f;
    particleLifeTime = 0.3f;

    drawBuffer = 1;
}

void Fire::Init() 
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<const char*>* transformFeedbackOutputs = new std::vector<const char*>();
    transformFeedbackOutputs->push_back("Position");
    transformFeedbackOutputs->push_back("Velocity");
    transformFeedbackOutputs->push_back("Age");

    shader.CreateFromFiles(
        "./Src/Chapter10/Fire/shaders/Fire.vert",
        "./Src/Chapter10/Fire/shaders/Fire.frag",
        "",
        "",
        "",
        transformFeedbackOutputs
    );

    InitBuffers();

    fire.LoadTexture("./Data/Textures/fire.png");

    glActiveTexture(GL_TEXTURE1);
    ParticleUtils::CreateRandomTex1D(nParticles * 2);

    shader.UseShader();
    shader.SetUniform("ParticleLifetime", particleLifeTime);
    shader.SetUniform("ParticleSize", particleSize);
    shader.SetUniform("Acceleration", glm::vec3(0.0f, 0.2f, 0.0f));
    shader.SetUniform("EmitterPosition", glm::vec3(0.0f));
    shader.SetUniform("EmitterBasis", ParticleUtils::MakeArbitaryBasis(glm::vec3(0.0f, 1.0f, 0.0f)));

}

void Fire::Render(glm::mat4& view, glm::mat4& projection) 
{
    shader.UseShader();
    fire.UseTexture(GL_TEXTURE0);

    shader.SetUniform("Time", time);
    shader.SetUniform("DeltaTime", deltaTime);

    shader.SetUniform("Pass", 1);

    glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuffer]);

        glBeginTransformFeedback(GL_POINTS);

            glBindVertexArray(particleArray[1 - drawBuffer]);
                glVertexAttribDivisor(0, 0);
                glVertexAttribDivisor(1, 0);
                glVertexAttribDivisor(2, 0);
                glDrawArrays(GL_POINTS, 0, nParticles);
            glBindVertexArray(0);

        glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    shader.SetUniform("Pass", 2);

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);

    glDepthMask(GL_FALSE);
        glBindVertexArray(particleArray[drawBuffer]);
            glVertexAttribDivisor(0, 1);
            glVertexAttribDivisor(1, 1);
            glVertexAttribDivisor(2, 1);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
        glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    drawBuffer = 1 - drawBuffer;
}

void Fire::Update(float deltaTime) 
{
    this->deltaTime = deltaTime;
    this->time += deltaTime;   
}

void Fire::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}

void Fire::InitBuffers() 
{
    
    glGenBuffers(2, positionBuffer);    
    glGenBuffers(2, velocityBuffer);    
    glGenBuffers(2, ageBuffer);    

    
    int size = nParticles * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

    
    std::vector<GLfloat> tempData(nParticles);
    float rate = particleLifeTime / nParticles;

    for (int i = 0; i < nParticles; i++) {
        tempData[i] = rate * (i - nParticles);
    }

    Random::shuffle(tempData);

    glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, tempData.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    glGenVertexArrays(2, particleArray);

    
    glBindVertexArray(particleArray[0]);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[0]);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

        
        glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[0]);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    
    glBindVertexArray(particleArray[1]);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[1]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[1]);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

        
        glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[1]);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    
    glGenTransformFeedbacks(2, feedback);

    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
        
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, positionBuffer[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velocityBuffer[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuffer[0]);

    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, positionBuffer[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velocityBuffer[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuffer[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

}
