#include "InstancedParticle.h"


Instanced::Instanced() 
{
    time = 0.0f;
    deltaTime = 0.0f;

    nParticles = 50;
    particleLifetime = 10.5f;
    emitterPosition = glm::vec3(0.0f);
    emitterDirection = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Instanced::Init() 
{
    std::vector<const char*>* transformFeedbackOutputs = new std::vector<const char*>();
    transformFeedbackOutputs->push_back("Position");
    transformFeedbackOutputs->push_back("Velocity");
    transformFeedbackOutputs->push_back("Age");
    transformFeedbackOutputs->push_back("Rotation");

    shader.CreateFromFiles(
        "./Src/Chapter10/InstancedParticle/shaders/InstancedParticle.vert",
        "./Src/Chapter10/InstancedParticle/shaders/InstancedParticle.frag",
        "",
        "",
        "",
        transformFeedbackOutputs
    );

    glEnable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    ParticleUtils::CreateRandomTex1D(nParticles * 4);

    torus.LoadModel("./Data/Torus.obj");

    InitBuffers();

    shader.UseShader();
    shader.SetUniform("ParticleLifetime", particleLifetime);
    shader.SetUniform("Acceleration", glm::vec3(0.0f, -0.4f, 0.0f));
    shader.SetUniform("EmitterPosition", emitterPosition);
    shader.SetUniform("EmitterBasis", ParticleUtils::MakeArbitaryBasis(emitterDirection));

    shader.SetUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    shader.SetUniform("Material.Kd", glm::vec3(0.9f, 0.5f, 0.2f));
    shader.SetUniform("Material.Ks", glm::vec3(0.95f, 0.55f, 0.95f));
    shader.SetUniform("Material.Ka", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetUniform("Material.E", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.SetUniform("Material.Shininess", 100.0f);

   

}

void Instanced::Update(float deltaTime) 
{
    this->deltaTime = deltaTime;
    this->time += deltaTime;
}

void Instanced::Render(glm::mat4& view, glm::mat4& projection) 
{
    shader.UseShader();

    shader.SetUniform("Time", time);
    shader.SetUniform("Pass", 1);
    shader.SetUniform("DeltaTime", deltaTime);

    glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuffer]);
            glBeginTransformFeedback(GL_POINTS);
                glBindVertexArray(particleArray[1 - drawBuffer]);
                    glDisableVertexAttribArray(0);
                    glDisableVertexAttribArray(1);

                    glVertexAttribDivisor(3, 0);
                    glVertexAttribDivisor(4, 0);
                    glVertexAttribDivisor(5, 0);
                    glVertexAttribDivisor(6, 0);

                    glDrawArrays(GL_POINTS, 0, nParticles);
                glBindVertexArray(0);
            glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    shader.SetUniform("Pass", 2);

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);

    glBindVertexArray(particleArray[drawBuffer]);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    
        glDrawArraysInstanced(GL_TRIANGLES, 0, torus.GetNumberOfVertices(), nParticles);

    glBindVertexArray(0);

    // torus.RenderModel();

    drawBuffer = 1 - drawBuffer;

}

void Instanced::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}

void Instanced::InitBuffers() 
{
    glGenBuffers(2, positionBuffer);
    glGenBuffers(2, velocityBuffer);
    glGenBuffers(2, ageBuffer);
    glGenBuffers(2, rotationBuffer);

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
    
    glBindBuffer(GL_ARRAY_BUFFER, rotationBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER, 2 * size, 0, GL_DYNAMIC_COPY);
    
    glBindBuffer(GL_ARRAY_BUFFER, rotationBuffer[1]);
        glBufferData(GL_ARRAY_BUFFER, 2 * size, 0, GL_DYNAMIC_COPY);

    
    std::vector<GLfloat> initialAges(nParticles);
    float rate = particleLifetime / nParticles;
    for (int i = 0; i < nParticles; i++) {
        initialAges[i] = rate * (i - nParticles);
    }

    glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), initialAges.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create vertex arrays for each set of buffers
    glGenVertexArrays(2, particleArray);
    
    // Setting up particle array 0
    glBindVertexArray(particleArray[0]);
        glBindBuffer(GL_ARRAY_BUFFER, torus.GetPositionBuffer());
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);    
            
        glBindBuffer(GL_ARRAY_BUFFER, torus.GetTextureBuffer());
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);    

        glBindBuffer(GL_ARRAY_BUFFER, torus.GetNormalBuffer());
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);    

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[0]);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(3);

        
        glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[0]);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(4);

        
        glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[0]);
            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(5);

        glBindBuffer(GL_ARRAY_BUFFER, rotationBuffer[0]);
            glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(6);

    glBindVertexArray(0);

    glBindVertexArray(particleArray[1]);
        glBindBuffer(GL_ARRAY_BUFFER, torus.GetPositionBuffer());
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);    
            
        glBindBuffer(GL_ARRAY_BUFFER, torus.GetTextureBuffer());
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);    

        glBindBuffer(GL_ARRAY_BUFFER, torus.GetNormalBuffer());
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);    

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[1]);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(3);

        
        glBindBuffer(GL_ARRAY_BUFFER, velocityBuffer[1]);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(4);

        
        glBindBuffer(GL_ARRAY_BUFFER, ageBuffer[1]);
            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(5);

        glBindBuffer(GL_ARRAY_BUFFER, rotationBuffer[1]);
            glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(6);

    glBindVertexArray(0);

    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);
    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, positionBuffer[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velocityBuffer[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuffer[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, rotationBuffer[0]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, positionBuffer[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velocityBuffer[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuffer[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, rotationBuffer[1]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

}
