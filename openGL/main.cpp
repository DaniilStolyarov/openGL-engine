#include "Shader.h"
#include "GameObject.h"
#include "Engine.h"
#include "Texture.h"
#include "Camera.h"
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

void testFunction();

using namespace glm;
using namespace irrklang;

	vector<GLfloat>* vertices /*= new vector<GLfloat>{
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
}*/;
vector<GLuint>* indices/* = new vector<GLuint>{
	0, 1, 3, 1, 2, 3, 4, 5, 7, 5, 6, 7, 8, 9, 11, 9, 10 , 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23
}*/;
Engine* myEngine;
GameObject* object;
ISoundEngine* SoundEngine;
GLfloat speed = 0.0f;
GLfloat fi = 0.0f;

// создаем тестовый игровой объект и помещаем его на сцену
void testGameObject()
{
	Texture* texture = new Texture("./Textures/rat-texture.jpeg");




	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, "./Shaders/vert.glsl");
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, "./Shaders/frag.glsl");
	object = new GameObject(vertices, indices, vertexShader, fragmentShader, texture);
	myEngine->scene.push_back(object);
}

void uniformCallback()
{
	mat4 rotation{ 1.0f };
	rotation = glm::rotate(rotation, glm::radians(fi), vec3{ 0.0f, 1.0f, 0.0f });
	GLint changeLoc = glGetUniformLocation(object->shaderProgram, "rotation");
	fi += speed*0.01f;
	glUniformMatrix4fv(changeLoc, 1, GL_FALSE, glm::value_ptr(rotation));

	Engine::camera->update(object);
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

	testFunction();

	testGameObject();

	object->setUniformCallback(&uniformCallback);
	
	myEngine->scene.push_back(object);
	SoundEngine->setSoundVolume(1.0f);

	ISound* sound = SoundEngine->play2D("./audio/rat.mp3", true);
	
	SoundEngine->setSoundVolume(0.2f);

	if (myEngine->mainLoop()) return 0;


	return 0;
}


void testFunction()
{
	using namespace processVerts;
	vertices = new vector<GLfloat>();
	indices = new vector<GLuint>();


	auto verts = getVerts();
	auto inds = getInds();
	vector<vec2>* texCoords = getTexCoords();
	Polygon p;

	unsigned int i_Iterator = 0;
	for (unsigned int i = 0; i < inds->size(); i++)
	{
	
		p = inds->at(i);
		if (p.Polygon_Type >= 3)
		{
			// position  ---   color   ---  texCoords
			// 3 * float --- 3 * float --- 2 * float
			
			/*cout << "first_triangle:" << endl;
			cout << "\ta: ";
			cout << "\tx: ";
			cout << texCoords->at(p.triangles[0].i_a.v_Indice).x;
			cout << "\ty:";
			cout << texCoords->at(p.triangles[0].i_a.v_Indice).y;

			cout << endl << "\t";
			cout << "b: ";
			cout << "\tx: ";
			cout << texCoords->at(p.triangles[0].i_b.v_Indice).x;
			cout << "\ty:";
			cout << texCoords->at(p.triangles[0].i_b.v_Indice).y;

			cout << endl << "\t";
			cout << "c: ";
			cout << "\tx: ";
			cout << texCoords->at(p.triangles[0].i_c.v_Indice).x;
			cout << "\ty:";
			cout << texCoords->at(p.triangles[0].i_c.v_Indice).y;*/

			//a
			vertices->push_back(verts->at(p.triangles[0].i_a.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_a.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_a.v_Indice - 1).Position.z);
			
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[0].i_a.v_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_a.v_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//b
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[0].i_b.v_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_b.v_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//c
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[0].i_c.v_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_c.v_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;
		}
		if (p.Polygon_Type == 4)
		{

			/*cout << endl;
			cout << "second_triangle:" << endl;
			cout << "\ta: ";
			cout << "\tx: ";
			cout << verts->at(p.triangles[1].i_a.v_Indice).Position.x;
			cout << "\ty:";
			cout << verts->at(p.triangles[1].i_a.v_Indice).Position.y;
			cout << "\tz:";
			cout << verts->at(p.triangles[1].i_a.v_Indice).Position.z;
			cout << endl << "\t";
			cout << "b: ";
			cout << "\tx: ";
			cout << verts->at(p.triangles[1].i_b.v_Indice).Position.x;
			cout << "\ty:";
			cout << verts->at(p.triangles[1].i_b.v_Indice).Position.y;
			cout << "\tz:";
			cout << verts->at(p.triangles[1].i_b.v_Indice).Position.z;
			cout << endl << "\t";
			cout << "c: ";
			cout << "\tx: ";
			cout << verts->at(p.triangles[1].i_c.v_Indice).Position.x;
			cout << "\ty:";
			cout << verts->at(p.triangles[1].i_c.v_Indice).Position.y;
			cout << "\tz:";
			cout << verts->at(p.triangles[1].i_c.v_Indice).Position.z;
			cout << endl << "\t";*/

			//a
			vertices->push_back(verts->at(p.triangles[1].i_a.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_a.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_a.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[1].i_a.v_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_a.v_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//b
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[1].i_b.v_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_b.v_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//c
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[1].i_c.v_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_c.v_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;
		}
	
	}


}