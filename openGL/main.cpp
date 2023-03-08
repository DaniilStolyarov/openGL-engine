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
	Texture* texture = new Texture("./Textures/rat-texture.jpg");




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


#include <sstream>

vector<GLfloat>* getVerts()
{
	vector<GLfloat>* verts = new vector<GLfloat>{};
	ifstream file_in("./Models/rat.obj");
	string line, word;
	istringstream _line;
	while (getline(file_in, line))
	{
		if (line[0] != 'v' || line[1] != ' ') continue;
		line = line.substr(2, line.size() - 2);
		_line = istringstream(line);
		
		while (getline(_line, word, ' '))
		{
			if (word.size() == 0) continue;
			verts->push_back(atof(word.c_str()));
		}
		verts->push_back(1.0f);
		verts->push_back(1.0f);
		verts->push_back(1.0f);

		verts->push_back(1.0f);
		verts->push_back(1.0f);
	}
	return verts;
}

vector<GLuint>* getInds()
{
	vector<GLuint>* inds = new vector<GLuint>{};
	ifstream file_in("./Models/rat.obj");
	string line, word, qWord;
	istringstream _line;
	vector<unsigned int> PolygonIndices{};
	vector<unsigned int> PolygonIndices_2{};
	unsigned int nWord;
	while (getline(file_in, line))
	{
		if (line[0] != 'f' || line[1] != ' ') continue;
		line = line.substr(2, line.size() - 2);
		_line = istringstream(line);
		PolygonIndices.clear();
		PolygonIndices_2.clear();
		while (getline(_line, word, ' '))
		{
			if (word.size() == 0) continue;
			qWord = "";
			for (int i = 0; i < word.size(); i++)
			{
				if (word[i] == '/') break;
				qWord = qWord + word[i];
				
			}
			nWord = atoi(qWord.c_str());
			PolygonIndices.push_back(nWord);
		}
		if (PolygonIndices.size() == 3)
		{
			inds->insert(inds->end(), PolygonIndices.begin(), PolygonIndices.end());
		}
		else if (PolygonIndices.size() == 4)
		{
			PolygonIndices_2.push_back(PolygonIndices[0]);
			PolygonIndices_2.push_back(PolygonIndices[2]);
			PolygonIndices_2.push_back(PolygonIndices[3]);
			PolygonIndices.pop_back();

		}
		inds->insert(inds->end(), PolygonIndices.begin(), PolygonIndices.end());
		inds->insert(inds->end(), PolygonIndices_2.begin(), PolygonIndices_2.end());
	}
	return inds;
}
vector<vec2>* getTexCoords() // список текстурных координат для вершин
{
	vector<vec2>* TexCrds = new vector<vec2>{};
	vector<float> tempCoords;
	ifstream file_in("./Models/rat.obj");
	string line, word, qWord;
	istringstream _line;

	while (getline(file_in, line))
	{
		if (line[0] != 'v' || line[1] != 't' || line[2] != ' ') continue;
		line = line.substr(3, line.size() - 3);
		_line = istringstream(line);
		
		tempCoords.clear();
		while (getline(_line, word, ' '))
		{
			if (word.size() == 0) continue;
			tempCoords.push_back(atof(word.c_str()));
		}
		TexCrds->push_back(vec2{ tempCoords[0], tempCoords[1] });
	}
	return TexCrds;
}
vector<GLuint>* getTexInds()
{
	vector<GLuint>* texInds = new vector<GLuint>{};
	ifstream file_in("./Models/rat.obj");
	string line, word, qWord;
	istringstream _line;
	vector<unsigned int> PolygonIndices{};
	vector<unsigned int> PolygonIndices_2{};
	unsigned int nWord;
	bool startRecording = false;
	while (getline(file_in, line))
	{
		if (line[0] != 'f' || line[1] != ' ') continue;
		line = line.substr(2, line.size() - 2);
		_line = istringstream(line);
		PolygonIndices.clear();
		PolygonIndices_2.clear();
		while (getline(_line, word, ' '))
		{
			if (word.size() == 0) continue;
			qWord = "";
			startRecording = false;
			for (int i = 0; i < word.size(); i++)
			{
				if (startRecording)
				{
					qWord += word[i];
				}
				if (word[i] == '/') startRecording = true;
			}
			nWord = atoi(qWord.c_str());
			PolygonIndices.push_back(nWord);
		}
		if (PolygonIndices.size() == 3)
		{
			texInds->insert(texInds->end(), PolygonIndices.begin(), PolygonIndices.end());
		}
		else if (PolygonIndices.size() == 4)
		{
			
			PolygonIndices_2.push_back(PolygonIndices[0]);
			PolygonIndices_2.push_back(PolygonIndices[2]);
			PolygonIndices_2.push_back(PolygonIndices[3]);
			PolygonIndices.pop_back();

		}
		texInds->insert(texInds->end(), PolygonIndices.begin(), PolygonIndices.end());
		texInds->insert(texInds->end(), PolygonIndices_2.begin(), PolygonIndices_2.end());
	}
	return texInds;
}
void testFunction()
{
	vertices = getVerts();
	indices = getInds();
	vector<vec2>* texCoords = getTexCoords();
	vector<GLuint>* texIndices = getTexInds();

}