#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	characterX = 0.0f;
	characterZ = 0.0f;

	cameraSelection = 1;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime, GLfloat xCharacter, GLfloat zCharacter)
{
	GLfloat velocity = moveSpeed * deltaTime;

	characterX = xCharacter;
	characterZ = zCharacter;
	positionTemp = glm::vec3(xCharacter - 5, 20.0f, zCharacter - 5);

	if (keys[GLFW_KEY_W])
	{
		switch (cameraSelection)
		{
		case 1:
			position += front * velocity;
			break;
		case 2:
			position += up * velocity;
			break;
		default:
			break;
		}
		
	}

	if (keys[GLFW_KEY_S])
	{
		switch (cameraSelection)
		{
		case 1:
			position -= front * velocity;
			break;
		case 2:
			position -= up * velocity;
			break;
		default:
			break;
		}
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	if (keys[GLFW_KEY_1])
	{
		cameraSelection = 1;
		printf("Camara 1");
	}

	if (keys[GLFW_KEY_2])
	{
		cameraSelection = 2;
		printf("Camara 2");
		position = glm::vec3(0.0f, 120.0f, 0.0f);
	}

	if (keys[GLFW_KEY_3])
	{
		cameraSelection = 3;
		printf("Camara 3");
	}

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	switch (cameraSelection)
	{
	case 1:
		yaw += xChange;
		pitch += yChange;
		break;
	case 2:
		yaw = -90.0f;
		pitch = -89.0f;
		break;
	case 3:
		yaw += xChange;
		pitch += yChange;
	default:
		break;
	}

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	if (cameraSelection != 3)
		position = position;
	else
		position = positionTemp;
}


Camera::~Camera()
{
}
