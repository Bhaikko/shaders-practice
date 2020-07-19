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

#include "./Helper/commonValues.cpp"

#include "./Helper/CoordinateAxes/CoordinateAxes.h"

Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

CoordinateAxes coordinateAxes;

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

    while (!mainWindow.GetShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.KeyControl(mainWindow.getKeys(), deltaTime);
        camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        mainWindow.SwapBuffers();
    }
    
}