#include "Shader.h"
#include "GameObject.h"
#include "Engine.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"
#include "Vertex.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <irrKlang/irrKlang.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "assimp_Imp.h"


using namespace glm;

using namespace irrklang;




/*	vector<GLfloat>* vertices = new vector<GLfloat>{
		// Позиции          // Цвета             // Текстурные координаты
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // Верхний левый

	 0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
	 0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
	-0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
	-0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // Верхний левый

	-0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	-0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

	0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
	0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

	 0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	 -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	 -0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

	  0.5f, 0.5f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	 0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	 -0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	 -0.5f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
};
vector<GLuint>* indices/* = new vector<GLuint>{
	0, 1, 3, 1, 2, 3, 4, 5, 7, 5, 6, 7, 8, 9, 11, 9, 10 , 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23
}*/;
Engine* myEngine;

ISoundEngine* SoundEngine;
GLfloat speed = 0.0f;
GLfloat fi = 0.0f;
void uniformCallback(GameObject* pThis);

// создаем тестовый игровой объект и помещаем его на сцену
void testGameObject()
{
	// rat
	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, "./Shaders/vert.glsl");
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, "./Shaders/frag.glsl");
	
	GameObject* rat = new GameObject("./Models/rat.obj", "./Textures/rat-texture.jpeg", vertexShader, fragmentShader);
	myEngine->scene.push_back(rat);
	rat->setUniformCallback(&uniformCallback);

	//rat teeth
	GameObject* teeth = new GameObject("./Models/teeth.obj", "./Textures/rat/teeth-texture.jpg", vertexShader, fragmentShader);
	myEngine->scene.push_back(teeth);
	teeth->setUniformCallback(&uniformCallback);
	
	GameObject* eyes = new GameObject("./Models/eyes.obj", "./Textures/rat/eyes-texture.jpg", vertexShader, fragmentShader);
	myEngine->scene.push_back(eyes);
	eyes->setUniformCallback(&uniformCallback);
}

void uniformCallback(GameObject* pThis)
{
	mat4 rotation{ 1.0f };
	rotation = glm::rotate(rotation, glm::radians(fi), vec3{ 0.0f, 1.0f, 0.0f });
	GLint changeLoc = glGetUniformLocation(pThis->shaderProgram, "rotation");
	fi += speed*0.01f;
	glUniformMatrix4fv(changeLoc, 1, GL_FALSE, glm::value_ptr(rotation));
	Engine::camera->update(pThis);
}
void linkInputRight()
{
	speed += 1.0f;
}
void linkInputLeft()
{
	speed -= 1.0f;
}
void linkKeys()
{
	myEngine->mapKeyCallback[GLFW_KEY_RIGHT] = &linkInputRight;
	myEngine->mapKeyCallback[GLFW_KEY_LEFT] = &linkInputLeft;
}
int main()
{

	myEngine = new Engine();
	Engine::camera = new Camera(vec3{ 0.0f, 1.6f, 2.0f }, vec3(25.0f, 0.0f, 0.0f)); // вращение по оси z не сработает
	// todo: rotation пока не работает
	SoundEngine = createIrrKlangDevice();

	linkKeys();


	testGameObject();



	SoundEngine->setSoundVolume(1.0f);

	ISound* sound = SoundEngine->play2D("./audio/rat.mp3", true);
	
	SoundEngine->setSoundVolume(0.2f);

	if (myEngine->mainLoop()) return 0;


	return 0;
}


