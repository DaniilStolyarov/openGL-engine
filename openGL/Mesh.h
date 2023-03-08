#pragma once
#include "Vertex.h"
#include <vector>
#include "Texture.h"
using namespace std;
class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

};