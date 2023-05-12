#include "Circle.h"

void Circle::calcVertices()
{
	mat4 rotateMat(1.0f);
	
	vec4 radiusVec{ 0.0f, 0.0f, 1.0f , 0.0f};
	
	vertices = {center};

	for (int i = 0; i < quality; i++)
	{
		rotateMat = rotate(rotateMat, glm::radians(360.0f / quality), vec3{ 0.0f, 1.0f, 0.0f });
	
		radiusVec = { 0.0f, 0.0f, 1.0f , 0.0f };
		radiusVec = rotateMat * radiusVec;
		vertices.push_back(vec3{radiusVec.x, radiusVec.y, radiusVec.z});
	}
	for (int i = 0; i < quality - 1; i++)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}
	indices.push_back(0);
	indices.push_back(quality);
	indices.push_back(1);


}

Circle::Circle()
{
	
}

Circle::Circle(float _radius, float x, float y, float z)
{
	this->center = vec3{ x, y, z };
	this->radius = _radius;
	calcVertices();
}

vector<float>* Circle::getVertices()
{
	vector<float>* GLvertices = new vector<float>;
	for (int i = 0; i < vertices.size(); i++)
	{
		GLvertices->push_back(vertices[i].x);
		GLvertices->push_back(vertices[i].y);
		GLvertices->push_back(vertices[i].z);

		GLvertices->push_back(1.0f);
		GLvertices->push_back(1.0f);
		GLvertices->push_back(1.0f);

		GLvertices->push_back(vertices[i].x / 2 - 0.5f);
		GLvertices->push_back(-vertices[i].z / 2 + 0.5f);
	}
	return GLvertices;
}

vector<unsigned int>* Circle::getIndices()
{
	return new vector<unsigned int>(indices);
}
