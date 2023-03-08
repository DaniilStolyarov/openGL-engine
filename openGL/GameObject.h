#pragma once

#include "Shader.h"
#include "Texture.h";
#include "Model.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// класс игровых объектов на сцене

class GameObject 
{
private:

	// трансформация координат по умолчанию
	void standardTransform();
public:
	GLuint shaderProgram; // Все шейдеры, которые собраны вместе
	GLuint VAO, VBO, EBO; // Буферы памяти на GPU
	vector<GLfloat>* vertices; // вершины объекта (в т.ч. цвета и текстурные координаты)
	vector<GLuint>* indices; // индексы вершин с нуля
	Shader* fragmentShader, *vertexShader; // шейдеры
	Texture* texture;
	// конструктор по умолчанию
	GameObject();

	// достаточный конструктор
	GameObject( vector<GLfloat>* vertices, vector<GLuint>* indices, Shader *vertexShader, Shader *fragmentShader, Texture* texture);

	GameObject(string pModel, string pTexture, Shader* vertexShader, Shader* fragmentShader);

	// лепим объект из тех материалов, которые уже есть на этот момент (вершины и шейдеры)
	void initGameObject();

	// обновляем состояние объекта в mainloop
	void update(); 

	// указатель на коллбек-функцию, которая будет вызвана в update
	// отвечает за передачу uniform значений в шейдер
	void (*shaderUniformCallback)(GameObject* pThis); 

	void setUniformCallback(void (*callback)(GameObject* pThis));

	
};

