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

#include "./Helper/commonValues.cpp"

#include "./Helper/CoordinateAxes/CoordinateAxes.h"
#include "./Helper/Skybox/Skybox.h"

// Source Files for Testing Shaders
// Chapter 2
// #include "./Src/Chapter02/UniformBlock/UniformBlock.h"

// Chapter 3
// #include "./Src/Chapter03/SceneDiscard/SceneDiscard.h"
// #include "./Src/Chapter03/sceneSubRoutine/sceneSubRoutine.h"
// #include "./Src/Chapter03/Fog/Fog.h"

// Chapter 4
// #include "./Src/Chapter04/PBR/PBR.h"
// #include "./Src/Chapter04/ToonShading/ToonShading.h"

// Chapter 5
// #include "./Src/Chapter05/AlphaTest/AlphaTest.h"
#include "./Src/Chapter05/NormalMap/NormalMap.h"


Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

CoordinateAxes coordinateAxes;
Skybox skybox;

// UniformBlock uniformBlock;
// SceneDiscard sceneDiscard;
// SceneSubRoutine sceneSubRoutine;
// Fog fog;

// PBR pbr;
// ToonShading toonShading;

// AlphaTest alphaTest;
NormalMap normalMap;

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

    coordinateAxes.InitialiseCoordinateAxes();
    skybox.Init();

    //  ################
    // uniformBlock.Init();
    
    // sceneDiscard.Init();
    // sceneSubRoutine.Init();
    // fog.Init();
    // pbr.Init();
    // toonShading.Init();
    // alphaTest.Init();
    normalMap.Init();
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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewMatrix = camera.CalculateViewMatrix();


        skybox.Render(viewMatrix, projectionMatrix);
        coordinateAxes.RenderCoordinateAxes(modelMatrix, projectionMatrix, viewMatrix);

        // uniformBlock.Render();
        // sceneDiscard.Render(modelMatrix, camera.CalculateViewMatrix(), projectionMatrix);
        // sceneSubRoutine.Render(modelMatrix, camera.CalculateViewMatrix(), projectionMatrix);
        // fog.Render(modelMatrix, camera.CalculateViewMatrix(), projectionMatrix);
        // pbr.Render(camera.CalculateViewMatrix(), projectionMatrix);
        // pbr.Update(deltaTime);
        // toonShading.Render(camera.CalculateViewMatrix(), projectionMatrix);
        // toonShading.Update(deltaTime);

        // alphaTest.Render(camera.CalculateViewMatrix(), projectionMatrix);
        // alphaTest.Update(deltaTime);

        normalMap.Render(viewMatrix, projectionMatrix);
        normalMap.Update(deltaTime);


        mainWindow.SwapBuffers();
    }
    
}