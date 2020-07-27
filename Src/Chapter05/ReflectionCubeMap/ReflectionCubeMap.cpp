#include "ReflectionCubeMap.h"

ReflectionCubeMap::ReflectionCubeMap()
{
    rotSpeed = 1.0f;
    angle = 0.0f;
}

void ReflectionCubeMap::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter05/ReflectionCubeMap/shaders/ReflectionCubeMap.vert",
        "./Src/Chapter05/ReflectionCubeMap/shaders/ReflectionCubeMap.frag"
    );

    std::string files[6];
    files[0] = "./Data/Textures/cubemap/pisa_posx.png";
    files[1] = "./Data/Textures/cubemap/pisa_negx.png";
    files[2] = "./Data/Textures/cubemap/pisa_posy.png";
    files[3] = "./Data/Textures/cubemap/pisa_negy.png";
    files[4] = "./Data/Textures/cubemap/pisa_posz.png";
    files[5] = "./Data/Textures/cubemap/pisa_negz.png";
    reflectionMap.LoadCubMap(files);

    teapot.LoadModel("./Data/Torus.obj");

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    shader.RegisterUniform("WorldCameraPosition");

    shader.RegisterUniform("ReflectFactor");
    shader.RegisterUniform("MaterialColor");

    shader.UseShader();
    glUniform1f(shader.GetUniformLocation("ReflectFactor"), 0.5f);
    glUniform4f(shader.GetUniformLocation("MaterialColor"), 0.5f, 0.5f, 0.5f, 1.0f);


}

void ReflectionCubeMap::Update(float deltaTime)
{
    angle += rotSpeed * deltaTime;
    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }
}

void ReflectionCubeMap::Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition)
{   
    shader.UseShader();

    reflectionMap.UseCubeMap(GL_TEXTURE0);

    glm::mat4 model(1.0f);
    // model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(model))));

    glUniform3f(shader.GetUniformLocation("WorldCameraPosition"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

    teapot.RenderModel();
}
