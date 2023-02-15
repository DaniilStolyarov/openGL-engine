#include "Camera.h"

Camera::Camera(vec3 _position, vec3 _rotation)
{
	this->position = _position;
	this->rotation = _rotation;

	cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = normalize(position - cameraTarget);
	up = vec3(0.0f, 1.0f, 0.0f);
	cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	cameraFront = vec3{ 0,0,-1.0f };
	cameraView = vec3{ 0,0,0 };

	yaw =  rotation.y - 90.0f; // вправо влево
	pitch = -rotation.x + 0.0f; // вверх вниз

	glm::vec3 front;
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	cameraFront = glm::normalize(front);
}

void Camera::update(GameObject* object)
{
	glm::mat4 view = glm::mat4(1.0f); // двигаем всю сцену от камеры, поворачиваем камеру вниз (поворачивая сцену вверх)
	view = glm::translate(view, position);


	view = glm::lookAt(position, position + cameraFront, vec3{0.0f, 1.0f, 0.0f});
	GLint viewLoc = glGetUniformLocation(object->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

}