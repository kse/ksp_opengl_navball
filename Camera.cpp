#include "Camera.hpp"

Camera::Camera() {
	glm::vec3 viewTarget  = glm::vec3(0.0f, 0.0f, 0.0f);
	upDirection    = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	viewDirection  = cameraPosition - viewTarget;

	//float cosangle = glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), glm::normalize(cameraPosition));
	//printf("Cosangle: %f\n", cosangle);

	yaw   = 270.0;
	pitch = 0.0;
}

void Camera::move(bool keys[]) {
	GLfloat speed = 0.05f;

	if (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]) {
			speed = 0.005f;
	}

	if (keys[GLFW_KEY_W]) {
		cameraPosition -= speed * viewDirection;
	}

	if (keys[GLFW_KEY_S]) {
		cameraPosition += speed * viewDirection;
	}

	if (keys[GLFW_KEY_A]) {
		cameraPosition -= glm::normalize(glm::cross(upDirection, viewDirection)) * speed;
	}

	if (keys[GLFW_KEY_D]) {
		cameraPosition += glm::normalize(glm::cross(upDirection, viewDirection)) * speed;
	}
}

glm::mat4 Camera::transformMatrix() {
	return glm::lookAt(cameraPosition, cameraPosition - viewDirection, upDirection);
}

void Camera::lookAround(double deltaX, double deltaY) {
	GLfloat sensitivity = 0.07f;

	deltaX *= sensitivity;
	deltaY *= sensitivity;

	yaw += deltaX;
	pitch += deltaY;

	glm::vec3 newViewDirection;
	newViewDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newViewDirection.y = sin(glm::radians(pitch));
	newViewDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	viewDirection = glm::normalize(newViewDirection);
}
