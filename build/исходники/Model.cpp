#include "Model.h"

bool toUseIndices = 0;

void pushTexCoords(vector<GLfloat>* _vertices, vector<vec2>* _texCoords, unsigned int index)
{
	if (index == 0)
	{
		_vertices->push_back(1.0f);
		_vertices->push_back(1.0f);
	}
	else
	{
		_vertices->push_back(_texCoords->at(index - 1).x);
		_vertices->push_back(_texCoords->at(index - 1).y);
	}
	
}

Model::Model(string pOBJ, string pTexture, GLfloat scale)
{
	texture = new Texture(pTexture);
	using namespace processVerts;
	vertices = new vector<GLfloat>();
	
	if (toUseIndices)
		indices = new vector<GLuint>();

	model_data = new modelData{};
	genModelData(model_data, pOBJ, scale);

	auto verts = model_data->verts;
	auto inds = model_data->inds;
	auto texCoords = model_data->TexCrds;

	Polygon p;

	unsigned int i_Iterator = 0;
	for (unsigned int i = 0; i < inds->size(); i++)
	{

		p = inds->at(i);
		if (p.Polygon_Type >= 3)
		{
			// position  ---   color   ---  texCoords
			// 3 * float --- 3 * float --- 2 * float
			//a
			vertices->push_back(verts->at(p.triangles[0].i_a.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_a.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_a.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			/*vertices->push_back(texCoords->at(p.triangles[0].i_a.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_a.t_Indice - 1).y);*/

			pushTexCoords(vertices, texCoords, p.triangles[0].i_a.t_Indice);


			if (toUseIndices)
			{
				indices->push_back(i_Iterator);
				i_Iterator += 1;
			}
				
			
			

			//b
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			/*vertices->push_back(texCoords->at(p.triangles[0].i_b.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_b.t_Indice - 1).y);*/
			pushTexCoords(vertices, texCoords, p.triangles[0].i_b.t_Indice);

			if (toUseIndices)
			{
				indices->push_back(i_Iterator);
				i_Iterator += 1;
			}
			

			//c
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			/*vertices->push_back(texCoords->at(p.triangles[0].i_c.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_c.t_Indice - 1).y);*/
			pushTexCoords(vertices, texCoords, p.triangles[0].i_c.t_Indice);

			if (toUseIndices)
			{
				indices->push_back(i_Iterator);
				i_Iterator += 1;
			}
		}
		if (p.Polygon_Type == 4)
		{
			//a
			vertices->push_back(verts->at(p.triangles[1].i_a.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_a.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_a.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			/*vertices->push_back(texCoords->at(p.triangles[1].i_a.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_a.t_Indice - 1).y);*/
			pushTexCoords(vertices, texCoords, p.triangles[1].i_a.t_Indice);

			if (toUseIndices)
			{
				indices->push_back(i_Iterator);
				i_Iterator += 1;
			}

			//b
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			/*vertices->push_back(texCoords->at(p.triangles[1].i_b.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_b.t_Indice - 1).y);*/
			pushTexCoords(vertices, texCoords, p.triangles[1].i_b.t_Indice);

			if (toUseIndices)
			{
				indices->push_back(i_Iterator);
				i_Iterator += 1;
			}

			//c
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			/*vertices->push_back(texCoords->at(p.triangles[1].i_c.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_c.t_Indice - 1).y);*/
			pushTexCoords(vertices, texCoords, p.triangles[1].i_c.t_Indice);

			if (toUseIndices)
			{
				indices->push_back(i_Iterator);
				i_Iterator += 1;
			}
		}

	}
}


