#include "Shader.h"
#include "GameObject.h"
#include "Engine.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"
#include "Vertex.h"
#include "Circle.h"

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

ISoundEngine* SoundEngine;
GLfloat speed_rotation_X = 0.0f;
GLfloat speed_rotation_Y = 0.0f;
GameObject* someObj;
void uniformCallback(GameObject* pThis);
void uniformCallback_ROTATEABLE(GameObject* pThis);
void uniformCallback_RotateProp(GameObject* pThis);
// дефолтный набор шейдеров 
Shader* vertexShader;
Shader* fragmentShader;

// для самолётика
int partsCount = 0;
// рисуем машинку
void drawCar() 
{
	setlocale(LC_ALL, "Rus");
	cout << "ВНИМАНИЕ! ИЗ-ЗА ОГРАНИЧЕНИЙ GITHUB, МНЕ ПРИШЛОСЬ СЖАТЬ МОДЕЛЬ МАШИНКИ В ZIP.\n ПОЖАЛУЙСТА, СПЕРВА ПЕРЕЙДИТЕ В ПОДПАПКУ openGL\\Models И РАСПАКУЙТЕ АРХИВ ПРЯМО ТУДА" << endl;
	cout << "ЕСЛИ ВЫ ГОТОВЫ, НАЖМИТЕ ЛЮБУЮ КНОПКУ" << endl;
	system("pause");
	setlocale(LC_ALL, "en-US");
	GameObject* car = new GameObject("./Models/Nisssa_Figaro_1991_OBJ.obj", "./Textures/Kudzh.jpg", vertexShader, fragmentShader, 50.0f);
	car->transform.position += vec3{0.0f, -2.0f, -5.0f};
	car->setUniformCallback(&uniformCallback_ROTATEABLE);
	myEngine->scene.push_back(car);
}

void drawRat()
{
	GameObject* rat = new GameObject("./Models/rat.obj", "./Textures/rat-texture.jpeg", vertexShader, fragmentShader, 1.0f);
	myEngine->scene.push_back(rat);
	rat->setUniformCallback(uniformCallback_ROTATEABLE);

	GameObject* teeth = new GameObject("./Models/teeth.obj", "./Textures/rat/teeth-texture.jpg", vertexShader, fragmentShader, 1.0f);
	myEngine->scene.push_back(teeth);
	teeth->setUniformCallback(uniformCallback_ROTATEABLE);

	GameObject* eyes = new GameObject("./Models/eyes.obj", "./Textures/rat/eyes-texture.jpg", vertexShader, fragmentShader, 1.0f);
	myEngine->scene.push_back(eyes);
	eyes->setUniformCallback(uniformCallback_ROTATEABLE);
}

void drawPlane()
{

	GameObject* plane = new GameObject("./Models/plane.obj", "./Textures/Kudzh.jpg", vertexShader, fragmentShader, 1.0f);
	GameObject* prop = plane->children[159]; // пропеллер
	
	plane->setUniformCallback(&uniformCallback_ROTATEABLE);
	/*myEngine->scene.push_back(plane);*/

	prop->setUniformCallback(&uniformCallback_RotateProp);
	
	// соединяем 159 и 160 в один объект (левая и правая часть винта)
	prop->vertices->insert(prop->vertices->end(),
		plane->children[160]->vertices->begin(), plane->children[160]->vertices->end());
	// добавляем также 162 (ротор винта)
	prop->vertices->insert(prop->vertices->end(),
		plane->children[162]->vertices->begin(), plane->children[162]->vertices->end());

	myEngine->scene.push_back(prop);
	someObj = plane;

	prop->initGameObject();
	
}

void drawTank()
{
	GameObject* tank = new GameObject("./Models/WeissMaus.obj", "./Textures/tank.jpg", vertexShader, fragmentShader, 1.0f);

	tank->setUniformCallback(&uniformCallback_ROTATEABLE);

	for (auto it = tank->children.begin(); it != tank->children.end(); it++)
	{
		myEngine->scene.push_back(*it);
		cout << "added " << (*it)->name << endl;
		(*it)->setUniformCallback(uniformCallback);
	}
	GameObject* tank_head = tank->children[4];
	tank_head->setUniformCallback(uniformCallback_ROTATEABLE);
	for (int i = 0; i < tank_head->vertices->size(); i++)
	{
		if (i % 8 == 2)  tank_head->vertices->at(i) += 1.0f;
	}
	for (int i = 0; i < 1200; i++)
	{
		tank_head->vertices->pop_back(); // убираем "артефакт" с модельки
	}
	tank_head->initGameObject();
	tank_head->translate(tank_head->transform.position + vec3(0.0f, 0.0f, -1.0f)); 

	someObj = tank->children[3];
}
// создаем тестовый игровой объект(ы) и помещаем его(их) на сцену
void testGameObject()
{

	vertexShader = new Shader(GL_VERTEX_SHADER, "./Shaders/vert.glsl");
	fragmentShader = new Shader(GL_FRAGMENT_SHADER, "./Shaders/frag.glsl");
	

	// drawPlane();
	// drawCar();

	// drawRat();

	drawTank();
	
}

void uniformCallback(GameObject* pThis)
{
	
	/*pThis->rotate(Y, speed_rotation_Y * 0.01f);
	pThis->rotate(X, speed_rotation_X * 0.01f);*/
	Engine::camera->update(pThis);
}
void uniformCallback_ROTATEABLE(GameObject* pThis)
{
	
	pThis->transform.rotation = vec3{ speed_rotation_X * 0.01f, speed_rotation_Y * 0.01f, 0 };
	
	
	Engine::camera->update(pThis);
}

void uniformCallback_RotateProp(GameObject* pThis)
{
	vec3 offset = (vec3{ -1.29193e-06f, 1.23342f, 3.17371f });
	pThis->transform.offset = offset;
	pThis->transform.rotation = vec3{ 0, 0, speed_rotation_Y * 0.1f };
	Engine::camera->update(pThis);
}
void linkInputRight()
{
	speed_rotation_Y -= 1.0f;
}
void linkInputLeft()
{
	speed_rotation_Y += 1.0f;
}
void linkInputUp()
{
	speed_rotation_X -= 1.0f;
}
void linkInputDown()
{
	speed_rotation_X += 1.0f;
}
void linkInputL()
{
	someObj->transform.position += vec3(0.01f, 0.0f, 0.0f);
}
void linkInputJ()
{
	someObj->transform.position -= vec3(0.01f, 0.0f, 0.0f);
}

void linkInputEQUAL()
{
	if (partsCount >= someObj->children.size()) return;
	GameObject* child = someObj->children[partsCount];

	if (partsCount != 159 && partsCount != 160 && partsCount != 162)
	{
		child->setUniformCallback(&uniformCallback);
		
		myEngine->scene.push_back(child);
	}
	child->transform.position += vec3(0.00f, -2.0f, -5.0f);
	cout << "pressed!" << partsCount << endl;
	//myEngine->keyBuffer[GLFW_KEY_EQUAL] = 0; // более "строгое" нажатие
	
	partsCount += 1;
}
void linkKeys()
{
	myEngine->mapKeyCallback[GLFW_KEY_RIGHT] = &linkInputRight;
	myEngine->mapKeyCallback[GLFW_KEY_LEFT] = &linkInputLeft;
	myEngine->mapKeyCallback[GLFW_KEY_UP] = &linkInputUp;
	myEngine->mapKeyCallback[GLFW_KEY_DOWN] = &linkInputDown;
	myEngine->mapKeyCallback[GLFW_KEY_L] = &linkInputL;
	myEngine->mapKeyCallback[GLFW_KEY_J] = &linkInputJ;
	myEngine->mapKeyCallback[GLFW_KEY_EQUAL] = &linkInputEQUAL;
}
int main()
{
	myEngine = new Engine(1);
	
	Engine::camera = new Camera(vec3{ 0.0f, 1.6f, 2.0f }, vec3(25.0f, 0.0f, 0.0f)); // вращение по оси z не сработает
	// todo: rotation пока не работает
	SoundEngine = createIrrKlangDevice();

	linkKeys();

	testGameObject();

	ISound* sound = SoundEngine->play2D("./audio/rat.mp3", true);
	
	SoundEngine->setSoundVolume(0.0f);

	if (myEngine->mainLoop()) return 0;

	return 0;
}


