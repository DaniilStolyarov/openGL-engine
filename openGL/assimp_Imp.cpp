#include "assimp_Imp.h"
#include <iostream>
using namespace std;
const aiScene* Assimp_Implementation::
loadModel(const std::string& pFile)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_Triangulate 
		/*| aiProcess_GenSmoothNormals 
		| aiProcess_FlipUVs 
		| aiProcess_CalcTangentSpace*/);

	// если что-то не так, сообщить об этом
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return nullptr;
	}
	return scene;
}