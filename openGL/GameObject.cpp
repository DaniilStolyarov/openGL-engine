#include "GameObject.h"




GameObject::GameObject() {
	shaderProgram = 0;
	VAO = 0, VBO = 0, EBO = 0;
	vertices = nullptr;
	indices = nullptr;
	fragmentShader = nullptr;
	vertexShader = nullptr;
	shaderUniformCallback = nullptr;
	texture = 0;
	transform = Transform();

}
GameObject::GameObject(vector<GLfloat>* vertices, vector<GLuint>* indices, Shader *vertexShader, Shader *fragmentShader, Texture* texture) {
	this->vertices = vertices;
	this->indices = indices;
	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;
	this->texture = texture;
	transform = Transform();
	initGameObject();
}

GameObject::GameObject(string pModel, string pTexture, Shader* vertexShader, Shader* fragmentShader, GLfloat scale)
{
	this->model = new Model(pModel, pTexture, scale);
	this->vertices = model->vertices;
	this->indices = model->indices;
	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;
	this->texture = model->texture;
	transform = Transform();

	fragment_GameObject();
	initGameObject();
}

void GameObject::initGameObject()
{
	// собираем шейдерную программу
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, this->vertexShader->glShader);
	glAttachShader(this->shaderProgram, this->fragmentShader->glShader);
	glLinkProgram(this->shaderProgram);
	glUseProgram(this->shaderProgram);
	glDeleteShader(this->fragmentShader->glShader);
	glDeleteShader(this->vertexShader->glShader);

	// выделяем память для VAO, VBO, EBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices->size() * sizeof(GLfloat), this->vertices->data(), GL_DYNAMIC_DRAW);


	if (this->indices != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices->size() * sizeof(GLuint), this->indices->data(), GL_DYNAMIC_DRAW);

	}
	
	/*
		1 - индекс последовательности. Например, вершины - 0, цвета - 1, текстуры - 2
		2 - сколько элементов последовательности подряд нужны
		3, 4 - ...
		5 - stride (отступ в байтах между подпоследовательностями отдельных категорий)
		6 - отступ от начала последовательности
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GameObject::update()
{
	glUseProgram(this->shaderProgram);

	// сюда надо вставить glUniform

	this->standardTransform();


	if (shaderUniformCallback != nullptr)
	shaderUniformCallback(this);

	if (this->texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, this->texture->glTexture);
	}

	glBindVertexArray(this->VAO);

	if (this->indices == nullptr) {
		glDrawArrays(GL_TRIANGLES, 0, vertices->size() / 3);
	} 
	else {
		glDrawElements(GL_TRIANGLES, indices->size() * sizeof(GLuint)/*36*/, GL_UNSIGNED_INT, 0);	
	}

	glBindVertexArray(0);
}

void GameObject::setUniformCallback(void (*callback)(GameObject* pThis))
{
	this->shaderUniformCallback = callback;
}

void GameObject::standardTransform()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(50.0f), (GLfloat)1920/1080,
		0.1f, 100.0f);
	GLint projectionLoc = glGetUniformLocation(this->shaderProgram,
		"projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// translate to the current pos
	translate(transform.position);
	rotate(transform.rotation);
}

void GameObject::rotate(_axis_ P, float degrees)
{
	transform.rotation += vec3{ float(P == _axis_::X) * degrees, float(P == _axis_::Y) * degrees, float(P == _axis_::Z) * degrees };
}
void GameObject::rotate(vec3 nextRotation)
{
	transform.rotationMat = glm::translate(transform.rotationMat, 
		transform.offset);
	transform.rotationMat = glm::rotate(transform.rotationMat, glm::radians(transform.rotation.x), vec3{ 1, 0, 0 }
	/*vec3{ float(P == _axis_::X), float(P == _axis_::Y), float(P == _axis_::Z) }*/);

	transform.rotationMat = glm::rotate(transform.rotationMat, glm::radians(transform.rotation.y), vec3{ 0, 1, 0 });
	transform.rotationMat = glm::rotate(transform.rotationMat, glm::radians(transform.rotation.z), vec3{ 0, 0, 1 });
	transform.rotationMat = glm::translate(transform.rotationMat,
		-transform.offset);
	
	GLint changeLoc = glGetUniformLocation(this->shaderProgram, "rotation");
	glUniformMatrix4fv(changeLoc, 1, GL_FALSE, glm::value_ptr(transform.rotationMat));
	
}



vector<GameObject*>* GameObject::fragment_GameObject()
{
	vector<meshIndex>* childInds = model->model_data->prefabIndexes;
	unsigned int Start = vertices->size();
	for (int i = childInds->size() - 1; i > -1; i--)
	{
		vector<GLfloat>* childVertices = new vector<GLfloat>();
		
;
		for (unsigned int vI = (childInds->at(i).startIndex + 1) * 24; vI < Start; vI++)
		{
		
			childVertices->push_back(vertices->at(vI));
			
		}
		cout << (childInds->at(i).startIndex) * 24 << " " << Start << endl;
	
		Start = (childInds->at(i).startIndex) * 24;

		GameObject* childObject = new GameObject(childVertices, nullptr, vertexShader, fragmentShader, texture);
		childObject->name = childInds->at(i).name;
		children.push_back(childObject);
	}
	return nullptr;
}

void GameObject::translate(vec3 nextPosition)
{
	transform.translationMat = glm::translate(mat4{1.0f}, nextPosition);
	transform.position = nextPosition;

	GLint changeLoc = glGetUniformLocation(this->shaderProgram, "translation");
	glUniformMatrix4fv(changeLoc, 1, GL_FALSE, glm::value_ptr(transform.translationMat));
}




