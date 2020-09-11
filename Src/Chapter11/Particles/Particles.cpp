#include "Particles.h"

Particle::Particle() 
{
    nParticles = glm::ivec3(50, 50, 50);
    totalParticles = nParticles.x * nParticles.y * nParticles.z;

    time = 0.0f;
    deltaTime = 0.0f;
    angle = 0.0f;
    speed = 15.0f;
    bh1 = glm::vec4(5.0f, 0.0f, 0.0f, 1.0f);
    bh2 = glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f);
}

void Particle::Init() 
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    renderShader.CreateFromFiles(
        "./Src/Chapter11/Particles/shaders/Particles.vert",
        "./Src/Chapter11/Particles/shaders/Particles.frag"
    );

    computeShader.CreateComputeShader("./Src/Chapter11/Particles/shaders/Particles.comp");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    InitBuffers();

}

void Particle::Render(glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 att1 = glm::vec3(rot * bh1);
    glm::vec3 att2 = glm::vec3(rot * bh2);

    // Execute Compute Shader
    computeShader.UseShader();
    computeShader.SetUniform("BlackHolePosition1", att1);
    computeShader.SetUniform("BlackHolePosition2", att2);

    // This is defining workgroups in each dimensions
    // The invocations within each workgroup is defined in Computer Shader itself
    glDispatchCompute(totalParticles / 1000, 1, 1);     // This executes the Compute Shader
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Draw Scene
    renderShader.UseShader();

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);

    glPointSize(3.0f);
    renderShader.SetUniform("Color", glm::vec4(0.0f, 0.0f, 0.0f, 0.2f));

    glBindVertexArray(particlesVAO);
        glDrawArrays(GL_POINTS, 0, totalParticles);
    glBindVertexArray(0);

    // Draw attractors
    glPointSize(10.0f);

    GLfloat data[] = {
        att1.x, att1.y, att1.z, 1.0f,
        att2.x, att2.y, att2.z, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, bhBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), data);

        renderShader.SetUniform("Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(bhVAO);
            glDrawArrays(GL_POINTS, 0, 2);
        glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Particle::Update(float deltaTime) 
{
    this->time += deltaTime;
    this->deltaTime = deltaTime;

    angle += speed * deltaTime;
    if (angle >= 360.0f) {
        angle -= 360.0f;
    }
}

void Particle::InitBuffers() 
{
    // Initial Position of particles
    std::vector<GLfloat> initPos;
    std::vector<GLfloat> initVelocity(totalParticles * 4, 0.0f);

    glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
    GLfloat dx = 2.0f / (nParticles.x - 1);
    GLfloat dy = 2.0f / (nParticles.y - 1);
    GLfloat dz = 2.0f / (nParticles.z - 1);

    // Creating particles at (0, 0, 0)
    glm::mat4 transf = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, -1.0f));

    for (int i = 0; i < nParticles.x; i++) {
        for (int j = 0; j < nParticles.y; j++) {
            for (int k = 0; k < nParticles.z; k++) {
                p.x = dx * i;
                p.y = dy * j;
                p.z = dz * k;
                p.w = 1.0f;

                p = transf * p;

                initPos.push_back(p.x);
                initPos.push_back(p.y);
                initPos.push_back(p.z);
                initPos.push_back(p.w);
            }
        }
    }

    // Buffers for Position and velocity
    GLuint buffers[2];
    glGenBuffers(2, buffers);
    GLuint positionBuffer = buffers[0];
    GLuint velocityBuffer = buffers[1];

    GLuint bufferSize = totalParticles * 4 * sizeof(GLfloat);

    // Position
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, &initPos[0], GL_DYNAMIC_DRAW);

    // Velocity
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocityBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, &initVelocity[0], GL_DYNAMIC_COPY);

    // Setting Up VAO
    glGenVertexArrays(1, &particlesVAO);
    glBindVertexArray(particlesVAO);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Set up a buffer and VAO for drawing the attractors (Black Holes)
    glGenBuffers(1, &bhBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bhBuffer);

        GLfloat data[] = {
            bh1.x, bh1.y, bh1.z, bh1.w,
            bh2.x, bh2.y, bh2.z, bh2.w
        };

        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bhBuffer);

    glGenVertexArrays(1, &bhVAO);
    glBindVertexArray(bhVAO);

        glBindBuffer(GL_ARRAY_BUFFER, bhBuffer);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}

void Particle::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    renderShader.SetUniform("ModelMatrix", model);
    renderShader.SetUniform("ViewMatrix", view);
    renderShader.SetUniform("ProjectionMatrix", projection);
    renderShader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}
