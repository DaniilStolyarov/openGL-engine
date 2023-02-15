#pragma once
#include "GameObject.h"
#include "Camera.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


// класс игрового движка
class Engine
{
private:
	// заносим клавиши в буфер чтобы потом отдельно обработать
	static void keyCallback(GLFWwindow* window, GLint key, GLint scancode,
		GLint action, GLint mode);

	void bindCameraKeys();



public:


	GLFWwindow* window; // окно приложения
	GLint width = 1920, height = 1080; // размеры окна в пикселях
	
	vector<GameObject*> scene = {}; // сцена, которая состоит из указателей на игровые объекты
	
	// текущая камера 
	static Camera* camera;
	static GLint keyBuffer[512]; // здесь мы храним клавиши которые были нажатыми на момент последнего события
	void(*mapKeyCallback[512])() = {nullptr};
	static double lastCursorPosX, lastCursorPosY;
	// настройка библиотеки glfw
	void startGL(); 

	// запуск библиотеки glew
	// если запуск провалился, вернёт false
	bool startGlew();

	// настройка viewPort`a окна
	void makeViewport();

	// запуск окна в ОС
	// если запуск провалился, вернёт false
	bool startWindow();



	static void	cursorCallback(GLFWwindow* window, double xpos, double ypos);

	// стандартный запуск движка
	// вернёт код ошибки
	int start();

	// главный цикл движка. Вызывает метод update() у всех объектов на сцене
	// вернёт 0, если цикл завершился без ошибок
	bool mainLoop();

	// конструктор по умолчанию. Вызовет метод start() 
	Engine();

	// обрабатываем текущий буфер с клавишами
	void bufferKeyCallback();


	
};