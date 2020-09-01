#include "ParticleSystem.h"


ParticleSystem::ParticleSystem() 
{
    time = 0.0f;
    particleLifetime = 5.5f;
    nParticles = 8000;
    emitterDir = glm::vec3(-1, 2, 0);
    emitterPos = glm::vec3(1, 0, 0);

}

void ParticleSystem::Init() 
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitBuffers();

    shader.CreateFromFiles(
        "./Src/Chapter10/ParticleSystem/shaders/ParticleSystem.vert",
        "./Src/Chapter10/ParticleSystem/shaders/ParticleSystem.frag"
    );

    bluewWater.LoadTexture("./Data/Textures/bluewater.png");

    shader.UseShader();
    shader.SetUniform("ParticleLifeTime", particleLifetime);
    shader.SetUniform("ParticleSize", 0.05f);
    shader.SetUniform("Gravity", glm::vec3(0.0f, -0.2f, 0.0f));
    shader.SetUniform("EmitterPos", emitterPos);
}

void ParticleSystem::Update(float deltaTime) 
{
    time += deltaTime;

    // if (time > INT64_MAX) {
    //     time = 0;
    // }   
}

void ParticleSystem::Render(glm::mat4& view, glm::mat4& projection) 
{
    shader.UseShader();
    bluewWater.UseTexture(GL_TEXTURE0);

    glm::mat4 model(1.0f);

    SetMatrices(model, view, projection);
    shader.SetUniform("Time", time);

    glDepthMask(GL_FALSE);
        glBindVertexArray(particles);
            // Draw multiple instances of a range of elements
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
        glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

void ParticleSystem::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}

void ParticleSystem::InitBuffers() 
{
    // Generating Buffer for initial velocity and start time
    glGenBuffers(1, &initVelocity);
    glGenBuffers(1, &startTime);

    // Allocate Space for buffers
    int size = nParticles * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVelocity);
    glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

        // Fill first velocity buffer with random velocities
        glm::mat3 emitterBasis = ParticleUtils::MakeArbitaryBasis(emitterDir);
        glm::vec3 v(0.0f);

        float velocity, theta, phi;
        std::vector<GLfloat> data(nParticles * 3);
        for (unsigned int i = 0; i < nParticles; i++) {
            theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, RandFloat());
            phi = glm::mix(0.0f, glm::two_pi<float>(), RandFloat());

            // This velocity is in local coordinates
            v.x = sinf(theta) * cosf(phi);
            v.y = cosf(theta);
            v.z = sinf(theta) * sinf(phi);
            velocity = glm::mix(1.25f, 1.5f, RandFloat());
            v = glm::normalize(emitterBasis * v) * velocity;

            data[3 * i]     = 0.0f;
            data[3 * i + 1] = v.y;
            data[3 * i + 2] = v.z;

            // printf("%f %f %f \n", v.x, v.y, v.z);
        }

        glBindBuffer(GL_ARRAY_BUFFER, initVelocity);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.data());

        // Filling start time buffer
        float rate = particleLifetime / nParticles;
        for (unsigned int i = 0; i < nParticles; i++) {
            data[i] = rate * i;
        }

        glBindBuffer(GL_ARRAY_BUFFER, startTime);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &particles);
    glBindVertexArray(particles);
        glBindBuffer(GL_ARRAY_BUFFER, initVelocity);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, startTime);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glVertexAttribDivisor(0, 1);
        glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);


}

float ParticleSystem::RandFloat() 
{
    return rand.nextFloat();
}
