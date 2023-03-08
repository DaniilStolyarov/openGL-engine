#include "Model.h"

Model::Model(string pOBJ, string pTexture)
{
	texture = new Texture(pTexture);
	using namespace processVerts;
	vertices = new vector<GLfloat>();
	indices = new vector<GLuint>();

	

	auto verts = getVerts(pOBJ);
	auto inds = getInds(pOBJ);
	auto texCoords = getTexCoords(pOBJ);
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

			vertices->push_back(texCoords->at(p.triangles[0].i_a.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_a.t_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//b
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_b.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[0].i_b.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_b.t_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//c
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[0].i_c.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[0].i_c.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[0].i_c.t_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;
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

			vertices->push_back(texCoords->at(p.triangles[1].i_a.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_a.t_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//b
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_b.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[1].i_b.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_b.t_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;

			//c
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.x);
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.y);
			vertices->push_back(verts->at(p.triangles[1].i_c.v_Indice - 1).Position.z);

			vertices->push_back(1.0f);
			vertices->push_back(1.0f);
			vertices->push_back(1.0f);

			vertices->push_back(texCoords->at(p.triangles[1].i_c.t_Indice - 1).x);
			vertices->push_back(texCoords->at(p.triangles[1].i_c.t_Indice - 1).y);

			indices->push_back(i_Iterator);
			i_Iterator += 1;
		}

	}
}


