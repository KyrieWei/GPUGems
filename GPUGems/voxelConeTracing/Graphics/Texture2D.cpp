#include "Texture2D.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../tools/stb_image.h"

Texture2D::Texture2D(const std::string _shaderTextureName, const std::string path, const bool generateMipmaps, const int force_channels) : shaderTextureSamplerName(_shaderTextureName)
{
	unsigned char* textureBuffer = stbi_load(path.c_str(), &width, &height, &channels, force_channels);
	if (textureBuffer == nullptr)
	{
		std::cerr << "ERROR: Failed to load image with path " << path << " into a texture." << std::endl;
	}
	else
	{
		std::cout << "Successfully loaded image with path '" << path << "'." << std::endl;
	}

	//generate texture on GPU
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Parameter options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//set texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//upload texture buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureBuffer);

	if (generateMipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &textureID);
}

void Texture2D::Activate(Shader& shader, int textureUnit)
{
	shader.use();
	shader.setInt(shaderTextureSamplerName, textureUnit);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}