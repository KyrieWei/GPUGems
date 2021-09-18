#pragma once

#include <string>
#include <vector>

#include "../../tools/Shader.h"

class Texture3D
{
public:
	unsigned char* textureBuffer = nullptr;
	GLuint textureID;

	void Activate(Shader& shader, const std::string& glSamplerName, const int textureUnit = GL_TEXTURE0);

	void Clear(GLfloat clearColor[4]);

	Texture3D(const std::vector<GLfloat>& textureBuffer, const int width, const int height, const int depth, const bool generateMipmaps = true);

private:
	int width, height, depth;
	std::vector<GLfloat> clearData;
};

