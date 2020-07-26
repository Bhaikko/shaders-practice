#include "Skybox.h"

Skybox::Skybox()
{
    VAO = 0;
}

void Skybox::InitCube()
{
    GLfloat skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Skybox::Init()
{
    shader.CreateFromFiles(
        "./Helper/Skybox/shaders/Skybox.vert",
        "./Helper/Skybox/shaders/Skybox.frag"
    );

    std::string files[6];
    files[0] = "./Data/Textures/cubemap/pisa_posx.png";
    files[1] = "./Data/Textures/cubemap/pisa_negx.png";
    files[2] = "./Data/Textures/cubemap/pisa_posy.png";
    files[3] = "./Data/Textures/cubemap/pisa_negy.png";
    files[4] = "./Data/Textures/cubemap/pisa_posz.png";
    files[5] = "./Data/Textures/cubemap/pisa_negz.png";
    cubeMap.LoadCubMap(files);

    InitCube();

    shader.RegisterUniform("ViewMatrix");    
    shader.RegisterUniform("ProjectionMatrix");    
    shader.RegisterUniform("Skybox");

    shader.UseShader();

    glUniform1i(shader.GetUniformLocation("Skybox"), 0);
    
}

void Skybox::Render(glm::mat4 view, glm::mat4 projection)
{
    shader.UseShader();

    view = glm::mat4(glm::mat3(view));

    glDepthMask(GL_FALSE);

    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));

    cubeMap.UseCubeMap(GL_TEXTURE0);

    glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);

}
