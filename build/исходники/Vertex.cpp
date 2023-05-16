#include "Vertex.h"
#include "GameObject.h"
#include <iostream>

GLfloat convertWordToFloat(string word)
{
	return atof(word.c_str());
}
void genVertex(modelData* _model, string line, GLfloat MS)
{
	string word = string();
	unsigned char wordCount = 0;
	Vertex vert;
	vert.Position = { 0, 0, 0 };
	for (unsigned char i = 2; i < line.size(); i++)
	{
		if (line[i] == ' ')
		{
			switch (wordCount)
			{
			case 0:
				vert.Position.x = convertWordToFloat(word);
				break;
			case 1:
				vert.Position.y = convertWordToFloat(word);
			default:
				break;
			}
			word.clear();
			wordCount += 1;
		}
		else
		{
			word.push_back(line[i]);
		}
	}
	vert.Position.z = convertWordToFloat(word);
	word.clear();
	vert.Position.x /= MS;
	vert.Position.y /= MS;
	vert.Position.z /= MS;
	_model->verts->push_back(vert);
}

void split(string str, char c, vector<string>* result, int startFrom = 0)
{
	string word = string();
	for (int i = startFrom; i < str.size(); i++)
	{
		if (str[i] == c)
		{
			if (word == "") word = "0";
			result->push_back(word);
			word.clear();
		}
		else
		{
			word += str[i];
		}
	}
	if (word == "") word = "0";
	result->push_back(word);
}

void genFace(modelData* model, string line)
{
	Polygon p; 
	Triangle tr;
	
	vector<string> splitted_by_Space;
	vector<string> splitted_by_Slash;

	split(line, ' ', &splitted_by_Space, 2);
	p.Polygon_Type = splitted_by_Space.size();
	TriangleIndice* _rawTI = new TriangleIndice[p.Polygon_Type];

	for (int i = 0; i < splitted_by_Space.size(); i++)
	{
		splitted_by_Slash.clear();
		split(splitted_by_Space.at(i), '/', &splitted_by_Slash, 0);
		_rawTI[i].v_Indice = stoul(splitted_by_Slash.at(0).c_str());
		_rawTI[i].t_Indice = stoul(splitted_by_Slash.at(1).c_str());
	}
	bool isOddTriangle = 0;
	for (int i = 0; i + 2 < p.Polygon_Type; i++)
	{
		if (isOddTriangle)
		{
			tr.i_a = _rawTI[i - 1];

		}
		else
		{
			tr.i_a = _rawTI[i];
		}
		tr.i_b = _rawTI[i + 1];
		tr.i_c = _rawTI[i + 2];
		p.triangles.push_back(tr);
		model->trianglesCount += 1;
		isOddTriangle = !isOddTriangle;
	}
	
	model->inds->push_back(p);
}
void genTexCoords(modelData* _model, string line)
{
	string word = string();
	vec2 texCoord = {};
	for (unsigned char i = 2; i < line.size(); i++)
	{
		if (line[i] == ' ')
		{
			texCoord.x = convertWordToFloat(word);
			word.clear();
		}
		else
		{
			word.push_back(line[i]);
		}
	}
	texCoord.y = convertWordToFloat(word);
	word.clear();

	_model->TexCrds->push_back(texCoord);
}
void processVerts::genModelData(modelData* _toWriteIn, string _fileName, GLfloat MS)
{
	_toWriteIn->trianglesCount = 0;
	_toWriteIn->verts = new vector<Vertex>{};
	_toWriteIn->inds = new vector<Polygon>{};
	_toWriteIn->TexCrds = new vector<vec2>{};
	_toWriteIn->prefabIndexes = new vector<meshIndex>{};
	GameObject* gameObj = new GameObject();
	ifstream file_in(_fileName);
	string line;

	while (getline(file_in, line))
	{
		if (line.size() > 255)
		{
			// "some line here is longer than 255 characters. Skipped";
			continue;
		}
			
		if (line[0] == 'o' && line[1] == ' ')
		{
			meshIndex mI;
			mI.name = line.substr(2, line.size() - 2);
			mI.startIndex = _toWriteIn->trianglesCount;
			_toWriteIn->prefabIndexes->push_back(mI);
		}
		else if (line[0] == 'v' && line[1] == ' ')
			genVertex(_toWriteIn, line, MS);
		else if (line[0] == 'f' && line[1] == ' ')
			genFace(_toWriteIn, line);
		else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
			genTexCoords(_toWriteIn, line);
	}
}

