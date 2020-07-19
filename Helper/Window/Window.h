#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int initialize();

	GLint getBufferWidth() { return bufferWidth;  }
	GLint getBufferHeight() { return bufferHeight;  }
	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

private:
	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

public:
	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
};

