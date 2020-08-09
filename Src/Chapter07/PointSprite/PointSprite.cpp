#include "PointSprite.h"

PointSprite::PointSprite()
{
    
}

void PointSprite::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter07/PointSprite/shaders/PointSprite.vert",
        "./Src/Chapter07/PointSprite/shaders/PointSprite.frag",
        "./Src/Chapter07/PointSprite/shaders/PointSprite.geom"
    );

    numSprites = 50;
    locations = new float[numSprites * 3];
    srand((unsigned int)time(0));

    for (int i = 0; i < numSprites; i++) {
        glm::vec3 point(
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f
        );

        locations[i * 3 + 0] = point.x;
        locations[i * 3 + 1] = point.y;
        locations[i * 3 + 2] = point.z;
    }

    GLuint handle;
    glGenBuffers(1, &handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(GLfloat), locations, GL_STATIC_DRAW);

    delete[] locations;

    glGenVertexArrays(1, &sprites);
    glBindVertexArray(sprites);

        glBindBuffer(GL_ARRAY_BUFFER, handle);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    flower.LoadTexture("./Data/Textures/flower.png");

    shader.UseShader();

    shader.SetUniform("Size2", 0.15f);    
}

void PointSprite::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();

    glm::mat4 model(1.0f);

    SetMatrices(model, view, projection);

    flower.UseTexture(GL_TEXTURE0);

    glBindVertexArray(sprites);
    glDrawArrays(GL_POINTS, 0, numSprites);

    glFinish();
}


void PointSprite::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(glm::inverseTranspose(view * model)));
}
