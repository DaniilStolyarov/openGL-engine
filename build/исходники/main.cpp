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


#include <chrono>
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
	
	setlocale(LC_ALL, "en-US");
	GameObject* car = new GameObject("./Models/Nisssa_Figaro_1991_OBJ.obj", "./Textures/Kudzh.jpg", vertexShader, fragmentShader, 50.0f);
	car->transform.position += vec3{0.0f, -2.0f, -5.0f};
	car->setUniformCallback(&uniformCallback_ROTATEABLE);
	myEngine->scene.push_back(car);

	someObj = car;
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

	someObj = eyes;
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

float Y_of_XZ(float x, float z)
{
	return (- x * x / 4.0f  + z * z / 4.0f) / 2.0f;

	// return sqrt(1 - x*x - z*z);
}
// рисует квадрат заданного ребра
void pushPointToVector(vector<float>* verts, vec3 point, float texCoordsScale = 1.0f)
{
	verts->push_back(point.x); verts->push_back(point.y); verts->push_back(point.z); // vertex
	verts->push_back(0); verts->push_back(0); verts->push_back(0); // rgb
	verts->push_back(point.x / texCoordsScale); verts->push_back(point.z / texCoordsScale); // texture coords
}
void drawPlain()
{
	float t = 3.0f;
	float step = 0.1f;
	vector<float>* plain_vertices = new vector<float>{};

		for (float x = -t; x < t; x += step)
		{
			for (float z = -t; z < t; z += step)
			{
				
				vec3 point1 = { x, Y_of_XZ(x, z), z};
				vec3 point2 = { x, Y_of_XZ(x, z + step), z + step };
				vec3 point3 = { x + step, Y_of_XZ(x + step, z + step), z + step };
				vec3 point4 = { x + step,Y_of_XZ(x + step, z), z };

				pushPointToVector(plain_vertices, point1, step);
				pushPointToVector(plain_vertices, point2, step);
				pushPointToVector(plain_vertices, point3, step);

				pushPointToVector(plain_vertices, point1, step);
				pushPointToVector(plain_vertices, point3, step);
				pushPointToVector(plain_vertices, point4, step);
			}
		}
	
	Texture* plain_texture = new Texture("./Textures/Kudzh.jpg");
	GameObject* plain = new GameObject(plain_vertices, nullptr, vertexShader, fragmentShader, plain_texture);
	plain->setUniformCallback(uniformCallback_ROTATEABLE);
	myEngine->scene.push_back(plain);
}


enum intersection_type { SIGN, DELTA };
vector<vec3> trace_ray_by_s(vec3 s, float tMax, float sensivity, intersection_type ETYPE, float F(float a, float b, float c))
{
	vector<vec3> result;
	vec3 x;
	float previous_Value_F, current_Value_F;
	vec3 previous_Solution;
	previous_Value_F = F(0, 0, 0);
	previous_Solution = { INT_MAX, 0, 0 }; // так мы показываем что его ещё не было

	for (float t = 0; t <= tMax; t += sensivity)
	{
		x = s * t;
		current_Value_F = F(x.x, x.y, x.z);
		// если функция "прошла" через 0
		if (((ETYPE == SIGN) && ((previous_Value_F < 0 && current_Value_F > 0)
			|| (previous_Value_F > 0 && current_Value_F < 0)))
			|| (((ETYPE == DELTA) && (abs(current_Value_F) < sensivity / 0.1f))))
		{
			previous_Solution = x;
			// пока что мы обрабатываем не более одного решения)
			// а стоп, уже обрабатываем все)
			result.push_back(x);
		}
		previous_Value_F = current_Value_F;
	}

	return result;
}
vector<GLfloat>* trace_ray(float step /* шаг в градусах */, 
	float sensivity /*чувствительность в единицах расстояний*/, 
	float tMax /* дальность прорисовки*/, intersection_type ETYPE, float F(float x, float y, float z))
{
	vector<GLfloat>* vertices = new vector<GLfloat>();
	vec3 s;
	mat4 U;
	mat4 V1, V2, V3, V4;

	vec3 p1 = vec3{ 0 }, p2 = vec3{ 0 }, p3 = vec3{ 0 }, p4 = vec3{ 0 };
	vector<vec3> vp1, vp2, vp3, vp4;
	// перебираем угловые координаты u и v 
	for (float u = -180.0f; u < 180.0f; u += step)
	{
		U = glm::rotate(mat4{ 1.0f }, glm::radians(u), vec3{ 0.0f, 1.0f, 0.0f });
		for (float v = -180; v < 180; v += step)
		{
			// V - это матрица, которая трансформирует вектор i в нужный нам 
			V1 = glm::rotate(U, glm::radians(v), vec3{ 1.0f, 0.0f, 0.0f });
			vp1 = trace_ray_by_s(vec4{ 1.0f, 0.0f, 0.0f, 1.0f } *V1, tMax, sensivity, ETYPE, F);

			for (auto p : vp1)
			{
				pushPointToVector(vertices, p, 0.0f);
			}
		}
	}
	return vertices;
}
float F(float x, float y, float z)
{
	// функция в виде f(x, y, z) = 0
	// return (x - 2) * (x - 2) + y * y + z * z - 1;
	//return (x) * (x) / 4 + y * y / 2 + z * z / 9 - 4;
	//return z - pow(x * y, 3) - pow(y * x, 3);
	// return -x * x / 4 + y * y / 9 - 2 * z;

	return z - (-x * y * pow(exp(1.0f), (-x * x - y * y)));
	return (x - 2) * (x - 2) + y * y + z * z - 9;
	return z - sin(x);
	return z - cos(abs(x) + abs(y)) - (abs(x) + abs(y));
	return z - cos(abs(x) - abs(y));
}
void drawSphere()
{
	//auto begin = chrono::steady_clock::now();
	//cout << vRay->size() << endl;
	//auto end = chrono::steady_clock::now();
	//auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
	//cout << "time left: " << elapsed_ms.count() << endl;
	//return 0;

	 auto vRay = trace_ray(1.0f, 0.01f, 10.0f, DELTA, F);
	// auto vRay = trace_ray(0.2f, 0.01f, 10.0f, SIGN);
	Texture* spere_texture = new Texture("./Textures/Kudzh.jpg");
	GameObject* sphere = new GameObject(vRay, nullptr, vertexShader, fragmentShader, spere_texture);
	sphere->setUniformCallback(uniformCallback_ROTATEABLE);
	myEngine->scene.push_back(sphere);
	sphere->renderType = GL_POINTS;

	cout << sphere->vertices->size() / 8 << endl;
}
float Function1(float x, float y, float z)
{
	return z - (-x * y * pow(exp(1.0f), (-x * x - y * y)));
}
void drawFunction1()
{
	auto vRay = trace_ray(1.0f, 0.01f, 10.0f, SIGN, Function1);
	// auto vRay = trace_ray(0.2f, 0.01f, 10.0f, SIGN);
	Texture* spere_texture = new Texture("./Textures/Kudzh.jpg");
	GameObject* sphere = new GameObject(vRay, nullptr, vertexShader, fragmentShader, spere_texture);
	sphere->setUniformCallback(uniformCallback_ROTATEABLE);
	myEngine->scene.push_back(sphere);
	sphere->renderType = GL_POINTS;
	cout << sphere->vertices->size() / 8 << endl;
}
float Function2(float x, float y, float z)
{
	return z - sin(x);
}
void drawFunction2()
{
	auto vRay = trace_ray(2.0f, 0.001f, 10.0f, DELTA, Function2);
	// auto vRay = trace_ray(0.2f, 0.01f, 10.0f, SIGN);
	Texture* spere_texture = new Texture("./Textures/Kudzh.jpg");
	GameObject* sphere = new GameObject(vRay, nullptr, vertexShader, fragmentShader, spere_texture);
	sphere->setUniformCallback(uniformCallback_ROTATEABLE);
	myEngine->scene.push_back(sphere);
	sphere->renderType = GL_POINTS;
	cout << sphere->vertices->size() / 8 << endl;
}
// создаем тестовый игровой объект(ы) и помещаем его(их) на сцену
void testGameObject(string entity)
{
	
	vertexShader = new Shader(GL_VERTEX_SHADER, "./Shaders/vert.glsl");
	fragmentShader = new Shader(GL_FRAGMENT_SHADER, "./Shaders/frag.glsl");
	
	


	if (entity == "plane")
		drawPlane();
	if (entity == "car")
		drawCar();
	if (entity == "tank")
		drawTank();
	if (entity == "rat")
		drawRat();
	if (entity == "plain")
		drawPlain();
	if (entity == "function1")
		drawFunction1();
	if (entity == "function2")
		drawFunction2();

	// drawPlane();
	// drawCar();

	// drawRat();

	// drawTank();
	
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
	if (someObj == nullptr) return;
	someObj->transform.position += vec3(0.01f, 0.0f, 0.0f);
}
void linkInputJ()
{
	if (someObj == nullptr) return;
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
	
	//myEngine->keyBuffer[GLFW_KEY_EQUAL] = 0; // более "строгое" нажатие
	
	partsCount += 1;
}
void linkInputDELETE()
{
	if (myEngine->scene.empty()) return;
	myEngine->scene.pop_back();

	myEngine->keyBuffer[GLFW_KEY_DELETE] = 0;
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
	myEngine->mapKeyCallback[GLFW_KEY_DELETE] = &linkInputDELETE;
}



int main()
{
	
	while (1)
{
	speed_rotation_X = 0.0f;
	speed_rotation_Y = 0.0f;
	cout << "Enter name of entity that you want to display.\nAvaliable objects: plane, car, tank, rat, plain.";
	string entity;
	cin >> entity;

	myEngine = new Engine(0, ivec2{1920, 1080});
	
	Engine::camera = new Camera(vec3{ 0.0f, 1.6f, 2.0f }, vec3(25.0f, 0.0f, 0.0f)); // вращение по оси z не сработает
	// todo: rotation пока не работает
	SoundEngine = createIrrKlangDevice();

	linkKeys();


	testGameObject(entity);

	SoundEngine->setSoundVolume(0.0f);
	ISound* sound = SoundEngine->play2D("./audio/rat.mp3", true);
	

	if (myEngine->mainLoop()) return 0;

	for (auto o : myEngine->scene)
	{
		delete o;
	}
	
	delete myEngine;
	delete sound;
	delete SoundEngine;
}
	return 0;
}


