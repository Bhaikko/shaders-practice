#include "Cloth.h"

#define PRIM_RESTART 0xffffff

Cloth::Cloth() 
{
    clothVAO = 0;
    numElements = 0;
    nParticles = glm::ivec2(40, 40);
    clothSize = glm::vec2(4.0f, 3.0f);

    time = 0.0f;
    deltaTime = 0.0f;
    speed = 200.0f;
    readBuffer = 0;
}

void Cloth::Init() 
{
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(PRIM_RESTART);

    renderShader.CreateFromFiles(
        "./Src/Chapter11/Cloth/shaders/Cloth.vert",
        "./Src/Chapter11/Cloth/shaders/Cloth.frag"
    );

    computeShader.CreateComputeShader("./Src/Chapter11/Cloth/shaders/Cloth.comp");
    computeShader.CreateComputeShader("./Src/Chapter11/Cloth/shaders/ClothNormal.comp");

    InitBuffers();

    renderShader.UseShader();
    renderShader.SetUniform("LightPosition", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    renderShader.SetUniform("LightIntensity", glm::vec3(1.0f));
    renderShader.SetUniform("Kd", glm::vec3(0.8f));
    renderShader.SetUniform("Ka", glm::vec3(0.2f));
    renderShader.SetUniform("Ks", glm::vec3(0.2f));
    renderShader.SetUniform("Shininess", 80.0f);

    computeShader.UseShader();
    float dx = clothSize.x / (nParticles.x - 1);
    float dy = clothSize.y / (nParticles.y - 1);

    computeShader.SetUniform("RestLengthHorizontal", dx);
    computeShader.SetUniform("RestLengthVertical", dy);
    computeShader.SetUniform("RestLengthDiagonal", sqrtf(dx * dx + dy * dy));

    clothTexture.LoadTexture("./Data/Textures/cloth.png", GL_RGB);
}

void Cloth::Update(float deltaTime) 
{
    this->time += deltaTime;
    this->deltaTime = deltaTime;
}

void Cloth::Render(glm::mat4& view, glm::mat4& projection) 
{
    clothTexture.UseTexture(GL_TEXTURE0);

    computeShader.UseShader();

    computeShader.SetUniform("DeltaTime", this->deltaTime);

    for (int i = 0; i < 1000; i++) {
        glDispatchCompute(nParticles.x / 10, nParticles.y / 10, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // Swap Buffers
        readBuffer = 1 - readBuffer;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBuffers[readBuffer]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionBuffers[1 - readBuffer]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velocityBuffer[readBuffer]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velocityBuffer[1 - readBuffer]);
    }

    computeNormalShader.UseShader();
    glDispatchCompute(nParticles.x / 10, nParticles.y / 10, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    renderShader.UseShader();

    glm::mat4 model(1.0f);
    SetMatrices(model, view, projection);

    glBindVertexArray(clothVAO);
        glDrawElements(GL_TRIANGLE_STRIP, numElements, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Cloth::InitBuffers() 
{
    glm::mat4 transf = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, clothSize.y, 0.0f));
    transf = glm::rotate(transf, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transf = glm::translate(transf, glm::vec3(0, -clothSize.y, 0.0f));

    // Initial Position of Particles
    std::vector<GLfloat> initialPositions;
    std::vector<GLfloat> initialVelocity(nParticles.x * nParticles.y * 4, 0.0f);
    std::vector<GLfloat> initialTextureCoords;

    float dx = clothSize.x / (nParticles.x - 1);
    float dy = clothSize.y / (nParticles.y - 1);
    float ds = 1.0f / (nParticles.x - 1);
    float dt = 1.0f / (nParticles.y - 1);

    glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);

    for (int i = 0; i < nParticles.y; i++) {
        for (int j = 0; j < nParticles.x; j++) {
            p.x = dx * j;
            p.y = dy * i;
            p.z = 0.0f;
            p = transf * p;

            initialPositions.push_back(p.x);
            initialPositions.push_back(p.y);
            initialPositions.push_back(p.z);
            initialPositions.push_back(1.0f);

            initialTextureCoords.push_back(ds * j);
            initialTextureCoords.push_back(dt * i);
        }
    }

    // Every row is one triangle strip
    std::vector<GLuint> el;
    for (int row = 0; row < nParticles.y - 1; row++) {
        for (int col = 0; col < nParticles.x; col++) {
            el.push_back(
                (row + 1) * nParticles.x +
                col
            );

            el.push_back(
                row * nParticles.x +
                col
            );
        }
        el.push_back(PRIM_RESTART);
    }

    // Generating Buffers
    GLuint buffers[7];
    glGenBuffers(7, buffers);
    positionBuffers[0] = buffers[0];
    positionBuffers[1] = buffers[1];

    velocityBuffer[0] = buffers[2];
    velocityBuffer[1] = buffers[3];

    normalBuffer = buffers[4];
    elementBuffer = buffers[5];
    textureBuffer = buffers[6];

    GLuint parts = nParticles.x * nParticles.y;

    // The buffers for positions
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBuffers[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), &initialPositions[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionBuffers[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

    // Velocities
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velocityBuffer[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), &initialVelocity[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velocityBuffer[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

    // Normal Buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, normalBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);

    // ELement Indices
    glBindBuffer(GL_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ARRAY_BUFFER, el.size() * sizeof(GLuint), &el[0], GL_DYNAMIC_COPY);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, initialTextureCoords.size() * sizeof(GLfloat), &initialTextureCoords[0], GL_STATIC_DRAW);


    numElements = GLuint(el.size());

    // Setting up the VAO
    glGenVertexArrays(1, &clothVAO);
    glBindVertexArray(clothVAO);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffers[0]);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBindVertexArray(0);        
}

void Cloth::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) 
{
    glm::mat4 mv = view * model;
    renderShader.SetUniform("ModelMatrix", model);
    renderShader.SetUniform("ViewMatrix", view);
    renderShader.SetUniform("ProjectionMatrix", projection);
    renderShader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])
    ));
}
