#pragma once
#include "Texture.h"
#include "Vertex.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
using namespace std;

class Model
{
public:
	vector<GLfloat>* vertices = nullptr;
	vector<GLuint>* indices = nullptr;
	Texture* texture = nullptr;

	string pFile_OBJ;
	string pFile_JPG;

	Model(string pOBJ, string pTexture);
};

