#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// класс игровых объектов на сцене
enum _axis_ { X, Y, Z };
struct Transform
{
	mat4 rotationMat;
	mat4 translationMat;
	vec3 position;
	vec3 rotation;
	vec3 offset = { 0, 0, 0 };
	Transform()
	{
		rotationMat = mat4{ 1.0f };
		translationMat = mat4{ 1.0f };
		position = vec3(0, 0, 0);
		rotation = vec3(0, 0, 0);

	}
};
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
	GLuint renderType = GL_TRIANGLES;
	vector<GameObject*> children;
	GameObject* parent;
	string name;
	Model* model;
	Shader* fragmentShader, *vertexShader; // шейдеры
	Texture* texture;
	Transform transform;
	GLfloat scale = 1.0f;
	// конструктор по умолчанию
	GameObject();

	// достаточный конструктор
	GameObject( vector<GLfloat>* vertices, vector<GLuint>* indices, 
		Shader *vertexShader, Shader *fragmentShader, Texture* texture);

	GameObject(string pModel, string pTexture,
		Shader* vertexShader, Shader* fragmentShader, GLfloat scale);

	~GameObject();
	// лепим объект из тех материалов, которые уже есть на этот момент (вершины и шейдеры)
	void initGameObject();
	
	vector<GameObject*>* fragment_GameObject(); // дробит объект на дочерние
	// обновляем состояние объекта в mainloop
	void update(); 

	// указатель на коллбек-функцию, которая будет вызвана в update
	// отвечает за передачу uniform значений в шейдер
	void (*shaderUniformCallback)(GameObject* pThis); 

	void setUniformCallback(void (*callback)(GameObject* pThis));

	void rotate(_axis_ P, float degrees);
	
	void rotate(vec3 nextRotation);
	void translate(vec3 nextPosition);
	

};





