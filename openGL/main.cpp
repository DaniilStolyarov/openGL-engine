#include "Shader.h"
#include "GameObject.h"
#include "Engine.h"
#include "Texture.h"
#include "Camera.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <irrKlang/irrKlang.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace irrklang;

Engine* myEngine;
GameObject* object;
ISoundEngine* SoundEngine;
GLfloat speed = 0.0f;
GLfloat fi = 0.0f;

// создаем тестовый игровой объект и помещаем его на сцену
void testGameObject()
{
	Texture* texture = new Texture("./Textures/rat.jpg");
	vector<GLfloat>* vertices = new vector<GLfloat>{
		// Позиции          // Цвета             // Текстурные координаты
	 0.75f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
	 0.75f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
	-0.75f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
	-0.75f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // Верхний левый

	 0.75f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
	 0.75f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
	-0.75f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
	-0.75f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // Верхний левый

	-0.75f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	-0.75f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
	-0.75f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.75f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

	0.75f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	0.75f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
	0.75f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	0.75f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

	 0.75f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	 0.75f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	 -0.75f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	 -0.75f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

	  0.75f, 0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	 0.75f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	 -0.75f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	 -0.75f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
	};
	vector<GLuint>* indices = new vector<GLuint>{
		0, 1, 3, 1, 2, 3, 4, 5, 7, 5, 6, 7, 8, 9, 11, 9, 10 , 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23
	};
	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, "./Shaders/vert.glsl");
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, "./Shaders/frag.glsl");
	object = new GameObject(vertices, indices, vertexShader, fragmentShader, texture);
	myEngine->scene.push_back(object);
}

void uniformCallback()
{
	GLint changeLoc = glGetUniformLocation(object->shaderProgram, "fi");
	fi += speed*0.001f;
	glUniform1f(changeLoc, fi);

	Engine::camera->update(object);
}
void linkInputRight()
{
	speed += 0.3f;
}
void linkInputLeft()
{
	speed -= 0.3f;
}
void linkKeys()
{
	myEngine->mapKeyCallback[GLFW_KEY_RIGHT] = &linkInputRight;
	myEngine->mapKeyCallback[GLFW_KEY_LEFT] = &linkInputLeft;
}
int main()
{
	myEngine = new Engine();
	Engine::camera = new Camera(vec3{ 0.0f, 2.0f, 2.0f }, vec3(45.0f, 0.0f, 0.0f)); // вращение по оси z не сработает
	// todo: rotation пока не работает
	SoundEngine = createIrrKlangDevice();

	linkKeys();

	testGameObject();

	object->setUniformCallback(&uniformCallback);
	
	myEngine->scene.push_back(object);
	
	ISound* sound = SoundEngine->play2D("./audio/rat.mp3", true);
	
	if (myEngine->mainLoop()) return 0;

	sound->drop();
	return 0;
}

