﻿#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sstream>
#include<vector>
#include <fstream>


using namespace glm;
using namespace std;
struct Vertex
{
	vec3 Position;
	vec2 TexCoords;
};

struct TriangleIndice
{
	GLuint v_Indice;
	GLuint t_Indice;
};


struct Triangle
{
	TriangleIndice i_a, i_b, i_c; 
	Vertex a, b, c; // вершины треугольника

};
struct Polygon
{
	GLuint Polygon_Type; // 4 - прямоугольник, 3 - треугольник
	vector<Triangle> triangles;
};

struct meshIndex
{
	unsigned int startIndex;
	string name;
};
struct modelData
{
	vector<Vertex>* verts = nullptr;
	vector<Polygon>* inds = nullptr;
	vector<vec2>* TexCrds = nullptr;
	vector<meshIndex>* prefabIndexes = nullptr;
	unsigned int trianglesCount;

};
namespace processVerts
{
	vector<Vertex>* getVerts(string pFile);
	vector<Polygon>* getInds(string pFile);
	vector<vec2>* getTexCoords(string pFile); // список текстурных координат для 
	void genModelData(modelData* _toWriteIn, string _fileName, GLfloat MS); // считывает verts, inds & texCoords в указанную структуру
}