#pragma once
#include <string>
#include <fstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;
// Класс для быстрой компиляции шейдеров из файлов. 
// Достаточно указать тип шейдера и путь к его исходному коду.
class Shader
{
public:
	string path; // путь к шейдеру
	string source; // исходный код шейдера
	GLuint glShader; // низкоуровневый шейдер OpenGL

	// получить исходный код из файла
	void getSourceFromFile(); 

	// достаточный конструктор
	Shader(int ShaderType, string path);

	// конструктор по умолчанию
	Shader(); 
};