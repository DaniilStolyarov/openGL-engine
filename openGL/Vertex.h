#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
struct Vertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};