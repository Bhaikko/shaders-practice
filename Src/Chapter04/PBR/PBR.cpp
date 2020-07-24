#include "PBR.h"

PBR::PBR()
{
    lightAngle = 0.0f;
    lightRotationSpeed = 1.5f;
    lightPos = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f);
    numBunny = 4;
}

void PBR::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter04/PBR/shaders/PBR.vert",
        "./Src/Chapter04/PBR/shaders/PBR.frag"
    );

    sphere.LoadModel("./Data/sphere.obj");
    terrain.LoadModel("./Data/terrain.obj");

    shader.RegisterUniform("ModelMatrix");
    shader.RegisterUniform("ViewMatrix");
    shader.RegisterUniform("ProjectionMatrix");
    shader.RegisterUniform("NormalMatrix");

    std::string uniformString = "";
    for (unsigned int i = 0; i < 3; i++) {
        uniformString = "Light[" + std::to_string(i) + "].L";
        shader.RegisterUniform(uniformString);
        
        uniformString = "Light[" + std::to_string(i) + "].Position";
        shader.RegisterUniform(uniformString);
    }

    shader.RegisterUniform("Material.Rough");
    shader.RegisterUniform("Material.Metal");
    shader.RegisterUniform("Material.Color");

    shader.UseShader();

    glUniform3f(shader.GetUniformLocation("Light[0].L"), 45.0f, 45.0f, 45.0f);
    glUniform3f(shader.GetUniformLocation("Light[1].L"), 0.3f, 0.3f, 0.3f);
    glUniform3f(shader.GetUniformLocation("Light[2].L"), 45.0f, 45.0f, 45.0f);

    glUniform4f(shader.GetUniformLocation("Light[1].Position"), 0, 0.15f, -1.0f, 0);

}

void PBR::Update(float deltaTime)
{
    lightAngle = glm::mod(lightAngle + deltaTime * lightRotationSpeed, glm::two_pi<float>());
    lightPos.x = glm::cos(lightAngle) * 7.0f;
    lightPos.y = 3.0f;
    lightPos.z = glm::sin(lightAngle) * 7.0f;

}

void PBR::Render(
    glm::mat4 view,
    glm::mat4 projection
) {
    shader.UseShader();

    glm::vec4 lightPos = view * this->lightPos;
    glUniform4f(shader.GetUniformLocation("Light[0].Position"), lightPos.x, lightPos.y, lightPos.z, lightPos.w);

    lightPos = view * glm::vec4(-7, 3, 7, 1);
    glUniform4f(shader.GetUniformLocation("Light[2].Position"), lightPos.x, lightPos.y, lightPos.z, lightPos.w);


    DrawFloor(view, projection);
    glm::vec3 baseColor(0.1f, 0.33f, 0.17f);

    for (unsigned int i = 0; i < numBunny; i++) {
        float bunnyX = i * (10.0f / (numBunny - 1)) - 5.0f;
        float rough = (i + 1) * (1.0f / numBunny);
        DrawSpot(glm::vec3(bunnyX, 0, 0), rough, 0, baseColor, view, projection);
    }

    float metalRough = 0.43f;

    // // Gold
    DrawSpot(glm::vec3(-10.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(1, 0.71f, 0.29f), view, projection);

    // Copper
    DrawSpot(glm::vec3(-5.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.64f, 0.54f), view, projection);

    // Aluminium
    DrawSpot(glm::vec3(0.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.91f, 0.92f, 0.92f), view, projection);

    // Titanium
    DrawSpot(glm::vec3(5.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.93f, 0.88f), view, projection);

    // Silver
    DrawSpot(glm::vec3(10.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.93f, 0.88f), view, projection);

}

void PBR::DrawFloor(glm::mat4 view, glm::mat4 projection)
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));

    glUniform1f(shader.GetUniformLocation("Material.Rough"), 0.9f);
    glUniform1f(shader.GetUniformLocation("Material.Metal"), 0);
    glUniform3f(shader.GetUniformLocation("Material.Color"), 0.2f, 0.2f, 0.2f);

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    terrain.RenderModel();
}

void PBR::DrawSpot(
    const glm::vec3& pos, 
    float rough, 
    int metal, 
    const glm::vec3& color,
    glm::mat4 view, 
    glm::mat4 projection
) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniform1f(shader.GetUniformLocation("Material.Rough"), rough);
    glUniform1f(shader.GetUniformLocation("Material.Metal"), metal);
    glUniform3f(shader.GetUniformLocation("Material.Color"), color.x, color.y, color.z);

    glUniformMatrix4fv(shader.GetUniformLocation("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(shader.GetUniformLocation("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * model))));

    sphere.RenderModel();
}