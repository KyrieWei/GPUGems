#pragma once

#include "Mesh.h"
#include "Shader.h"

class Quad
{
public:
	std::vector<Mesh> meshes;

	Quad()
	{
		std::vector<Vertex> vertexs;

		Vertex v;

		v.Position = { -1, -1, 1 };
		v.TexCoords = { 0, 1 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		v.Position = { 1, -1, 1 };
		v.TexCoords = { 0, 0 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		v.Position = { 1, 1, 1 };
		v.TexCoords = { 1, 1 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		v.Position = { -1, 1, 1 };
		v.TexCoords = { 1, 0 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };
		std::vector<Texture> textures;

		meshes.push_back(Mesh(vertexs, indices, textures));
	}

	void Draw(Shader& shader)
	{
		for (int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}
};