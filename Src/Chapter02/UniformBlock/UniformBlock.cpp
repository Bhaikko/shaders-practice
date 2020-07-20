#include "UniformBlock.h"

UniformBlock::UniformBlock()
{
    angle = 0.0f;
    shader = new Shader();
}

void UniformBlock::Init()
{
    shader->CreateFromFiles(
        "./Src/Chapter02/UniformBlock/shaders/UniformBlock.vert",
        "./Src/Chapter02/UniformBlock/shaders/UniformBlock.frag"
    );

    shaderProgram = shader->GetShaderProgramID();

    InitUniformBuffer();

    // Initilising Geometry
    GLfloat positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.8f,  0.8f, 0.0f,
        -0.8f, -0.8f, 0.0f,
         0.8f,  0.8f, 0.0f,
        -0.8f,  0.8f, 0.0f
    };

    GLfloat tcData[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);

    GLuint positionBufferHandle = vboHandles[0];
    GLuint tcBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tcData, GL_STATIC_DRAW);

    // Setting up VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

        glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindVertexArray(0);

}

void UniformBlock::Update(float t)
{
    angle += 1.0f;
    if (angle >= 360.0f) {
        angle -= 360.0f;
    }
}

void UniformBlock::Render()
{
    shader->UseShader();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void UniformBlock::InitUniformBuffer()
{
    // Get index of uniform block
    GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "BlobSettings");

    // Allocate Space for buffer
    GLint blockSize;
    glGetActiveUniformBlockiv(
        shaderProgram, 
        blockIndex, 
        GL_UNIFORM_BLOCK_DATA_SIZE, 
        &blockSize
    );
    
    GLubyte* blockBuffer;
    blockBuffer = (GLubyte*) malloc(blockSize);

    // Query for offsets of each block variable
    const GLchar* names[] = {
        "BlobSettings.InnerColor",
        "BlobSettings.OuterColor",
        "BlobSettings.RadiusInner",
        "BlobSettings.RadiusOuter"
    };

    GLuint indices[4];
    glGetUniformIndices(shaderProgram, 4, names, indices);

    GLint offset[4];
    glGetActiveUniformsiv(shaderProgram, 4, indices, GL_UNIFORM_OFFSET, offset);

    // Store Data within buffer at appropriate offsets
    GLfloat outerColor[] = {
        1.0f, 0.0f, 0.0f, 0.0f
    };

    GLfloat innerColor[] = {
        0.0f, 0.0f, 1.0f, 1.0f
    };

    GLfloat innerRadius = 0.25f, outerRadius = 0.45f;

    memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
    memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

    // Create buffer object and copy data
    GLuint uboHandle;
    glGenBuffers(1, &uboHandle);
    glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

    // Bind the buffer object to uniform buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboHandle);


}