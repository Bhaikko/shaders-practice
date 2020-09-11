#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Helper/Window/Window.h"
#include "./Helper/Camera/Camera.h"
#include "./Helper/Shader/Shader.h"
#include "./Helper/Grid/Grid.h"

#include "./Helper/commonValues.cpp"

#include "./Helper/CoordinateAxes/CoordinateAxes.h"
#include "./Helper/Skybox/Skybox.h"

// Source Files for Testing Shaders
#include "./Src/Chapter11/Particles/Particles.h"

GLint width = 1366, height = 768;

Window mainWindow(width, height);
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// CoordinateAxes coordinateAxes;
// Skybox skybox;
Grid grid(20, 20);

Particle particleSystem;

glm::mat4 projectionMatrix(1.0f);

int main(int argc, const char* argv[])
{
    mainWindow.initialize();

    camera = Camera(
        glm::vec3(0.53f, 1.24f, 1.90f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -100.0f,
        -24.0f,
        1.0f,
        1.0f
    );

    projectionMatrix = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

    glm::mat4 modelMatrix(1.0f);
    glm::mat4 viewMatrix(1.0f);

    // coordinateAxes.InitialiseCoordinateAxes();
    // skybox.Init();
    grid.Init();

    //  ################
    particleSystem.Init();
    //  ################

    glEnable(GL_DEPTH_TEST);
    
    std::cout << "[+] Scene Init Done" << std::endl;

    while (!mainWindow.GetShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.KeyControl(mainWindow.getKeys(), deltaTime);
        camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        viewMatrix = camera.CalculateViewMatrix();
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // skybox.Render(viewMatrix, projectionMatrix);
        // coordinateAxes.RenderCoordinateAxes(modelMatrix, projectionMatrix, viewMatrix);
        grid.Render(viewMatrix, projectionMatrix);

        particleSystem.Render(viewMatrix, projectionMatrix);
        particleSystem.Update(deltaTime);
        
        mainWindow.SwapBuffers();
    }
    
}