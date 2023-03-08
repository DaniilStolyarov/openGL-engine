#include "Vertex.h"
#include <iostream>



vector<Vertex>* processVerts::getVerts(string pFile)
{
	vector<Vertex>* verts = new vector<Vertex>{};
	ifstream file_in(pFile);
	string line, word;
	istringstream _line;
	vector<GLfloat> vBuffer;
	Vertex v;
	while (getline(file_in, line))
	{
		if (line[0] != 'v' || line[1] != ' ') continue;
		line = line.substr(2, line.size() - 2);
		_line = istringstream(line);

		vBuffer.clear();
		while (getline(_line, word, ' '))
		{
			if (word.size() == 0) continue;
			vBuffer.push_back(atof(word.c_str()));
		}
		v.Position.x = vBuffer[0]; v.Position.y = vBuffer[1]; v.Position.z = vBuffer[2];
		verts->push_back(v);
	}
	return verts;
}

vector<Polygon>* processVerts::getInds(string pFile)
{
	vector<Polygon>* inds = new vector<Polygon>{};
	ifstream file_in(pFile);
	string line, word, qWord, tWord;
	istringstream _line;

	vector<TriangleIndice> rawTriangleIndices{};
	unsigned int nWord, tnWord;

	while (getline(file_in, line))
	{
		if (line[0] != 'f' || line[1] != ' ') continue;
		line = line.substr(2, line.size() - 2);
		_line = istringstream(line);
		rawTriangleIndices.clear();
		
		while (getline(_line, word, ' '))
		{
			if (word.size() == 0) continue;
			qWord = "";
			tWord = "";
			bool startTexRecording = false;
			for (int i = 0; i < word.size(); i++)
			{
				if (word[i] == '/')
				{
					startTexRecording = true;
					continue;
				}
				if (startTexRecording == false)
				{
					qWord += word[i];
				}
				else
				{
					tWord += word[i];
				}
				
			}
			nWord = atoi(qWord.c_str());
			tnWord = atoi(tWord.c_str());
			//cout << nWord << " " << tnWord << endl;
			rawTriangleIndices.push_back(TriangleIndice{ nWord, tnWord });
		}
		if (rawTriangleIndices.size() == 3)
		{
			Polygon _p; Triangle _tr;
			_tr.i_a = rawTriangleIndices[0];
			_tr.i_b = rawTriangleIndices[1];
			_tr.i_c = rawTriangleIndices[2];

			_p.Polygon_Type = 3;
			_p.triangles.push_back(_tr);
			inds->push_back(_p);
		}
		else if (rawTriangleIndices.size() == 4)
		{
			Polygon _p; Triangle _tr1, _tr2;
			_tr1.i_a = rawTriangleIndices[0];
			_tr1.i_b = rawTriangleIndices[1];
			_tr1.i_c = rawTriangleIndices[2];

			_tr2.i_a = rawTriangleIndices[0];
			_tr2.i_b = rawTriangleIndices[2];
			_tr2.i_c = rawTriangleIndices[3];

			_p.Polygon_Type = 4;
			_p.triangles.push_back(_tr1);
			_p.triangles.push_back(_tr2);
			inds->push_back(_p);
		}
		
	}
	return inds;
}
vector<vec2>* processVerts::getTexCoords(string pFile) // список текстурных координат для вершин
{
	vector<vec2>* TexCrds = new vector<vec2>{};
	vector<float> tempCoords;
	ifstream file_in(pFile);
	string line, word, qWord;
	istringstream _line;

	while (getline(file_in, line))
	{
		if (line[0] != 'v' || line[1] != 't' || line[2] != ' ') continue;
		line = line.substr(3, line.size() - 3);
		_line = istringstream(line);

		tempCoords.clear();
		while (getline(_line, word, ' '))
		{
			if (word.size() == 0) continue;
			tempCoords.push_back(atof(word.c_str()));
		}
		TexCrds->push_back(vec2{ tempCoords[0], tempCoords[1] });
	}
	return TexCrds;
}
//vector<GLuint>* processVerts::getTexInds()
//{
//	vector<GLuint>* texInds = new vector<GLuint>{};
//	ifstream file_in("./Models/rat.obj");
//	string line, word, qWord;
//	istringstream _line;
//	vector<unsigned int> PolygonIndices{};
//	vector<unsigned int> PolygonIndices_2{};
//	unsigned int nWord;
//	bool startRecording = false;
//	while (getline(file_in, line))
//	{
//		if (line[0] != 'f' || line[1] != ' ') continue;
//		line = line.substr(2, line.size() - 2);
//		_line = istringstream(line);
//		PolygonIndices.clear();
//		PolygonIndices_2.clear();
//		while (getline(_line, word, ' '))
//		{
//			if (word.size() == 0) continue;
//			qWord = "";
//			startRecording = false;
//			for (int i = 0; i < word.size(); i++)
//			{
//				if (startRecording)
//				{
//					qWord += word[i];
//				}
//				if (word[i] == '/') startRecording = true;
//			}
//			nWord = atoi(qWord.c_str());
//			PolygonIndices.push_back(nWord);
//		}
//		if (PolygonIndices.size() == 3)
//		{
//			texInds->insert(texInds->end(), PolygonIndices.begin(), PolygonIndices.end());
//		}
//		else if (PolygonIndices.size() == 4)
//		{
//
//			PolygonIndices_2.push_back(PolygonIndices[0]);
//			PolygonIndices_2.push_back(PolygonIndices[2]);
//			PolygonIndices_2.push_back(PolygonIndices[3]);
//			PolygonIndices.pop_back();
//
//		}
//		texInds->insert(texInds->end(), PolygonIndices.begin(), PolygonIndices.end());
//		texInds->insert(texInds->end(), PolygonIndices_2.begin(), PolygonIndices_2.end());
//	}
//	return texInds;
//}