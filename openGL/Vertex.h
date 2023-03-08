#pragma once
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

namespace processVerts
{
	vector<Vertex>* getVerts();
	vector<Polygon>* getInds();
	vector<vec2>* getTexCoords(); // список текстурных координат для 
	vector<GLuint>* getTexInds();
}