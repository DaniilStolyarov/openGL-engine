#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// класс для своих функций Assimp`a
class Assimp_Implementation
{
public:
	// возвращает указатель на assimp-scene или nullptr
	static const aiScene* loadModel(const std::string& pFile);
};
