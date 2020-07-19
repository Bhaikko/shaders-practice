#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed);
	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	float distanceFromPlayer;
	float angleAroundOrigin;

	void update();

	public:
		void KeyControl(bool* keys, GLfloat deltaTime);
		void MouseControl(GLfloat xChange, GLfloat yChange);
		glm::mat4 CalculateViewMatrix();
		glm::vec3 GetCameraPosition() { return this->position; }
		glm::vec3 GetCameraDirection() { return glm::normalize(this->front); }

};

