#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <string>
// класс-обёртка для текстур

struct TextureStruct
{
	GLuint glTexture;
	std::string textureType;
};

class Texture
{
public:
	GLuint glTexture; // доступ к текстуре openGL
	std::string path; // путь к файлу-источнику
	GLint width, height; // размеры исходной картинки
	unsigned char* image; // массив байт из картинки, пригодный для создания текстуры
	TextureStruct textureStruct; // структура для передачи openGL

	Texture(std::string path);
	
};