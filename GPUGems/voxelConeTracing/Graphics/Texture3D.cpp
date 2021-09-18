#include "Texture3D.h"

Texture3D::Texture3D(const std::vector<GLfloat>& textureBuffer, const int _width, const int _height, const int _depth, const bool generateMipmaps) :
	width(_width), height(_height), depth(_depth), clearData(4 * _width * _height * _depth, 0.0f)
{
	//generate texture on GPU
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);

	//parameter options
	const auto wrap = GL_CLAMP_TO_BORDER;
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);

	const auto filter = GL_LINEAR_MIPMAP_LINEAR;
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//upload texture buffer
	const int levels = 7;
	glTexStorage3D(GL_TEXTURE_3D, levels, GL_RGBA, width, height, depth);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_FLOAT, &textureBuffer[0]);
	if (generateMipmaps)
		glGenerateMipmap(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::Activate(Shader& shader, const std::string& glSamplerName, const int textureUnit)
{
	shader.use();
	shader.setInt(glSamplerName, textureUnit);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_3D, textureID);
}

void Texture3D::Clear(GLfloat clearColor[4])
{
	GLint previousBoundTextureID;
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousBoundTextureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glClearTexImage(textureID, 0, GL_RGBA, GL_FLOAT, &clearColor);
	glBindTexture(GL_TEXTURE_3D, previousBoundTextureID);
}