#pragma once
#include "GameObject.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// класс камеры
using namespace glm;
class Camera 
{
public:
	// глобальные усечённые нормализованные координаты
	vec3 position;
	vec3 rotation;

	// фокус камеры
	vec3 cameraTarget;
	// вектор направления камеры
	vec3 cameraDirection;
	// вектор оси y камеры
	vec3 cameraUp;
	// вектор оси x камеры
	vec3 cameraRight;
	// вектор который просто смотрит наверх
	vec3 up;
	// вектор для функции lookAt()
	vec3 cameraView;
	
	GLfloat yaw, pitch;

	vec3 cameraFront;

	// достаточный конструктор
	Camera(vec3 _position, vec3 _rotation);


	// обновляет данные камеры
	// метод update() вызывается в mainloop
	void update(GameObject* object);
};