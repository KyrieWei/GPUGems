#pragma once
#include <string>
#include "../../tools/Shader.h"

//#include <GLFW/glfw3.h>

class Texture2D
{
public:
	std::string shaderTextureSamplerName;
	GLuint textureID;


	Texture2D(const std::string shaderTextureSamplerName, const std::string path, const bool generateMipmaps = true, const int force_channels = 0);
	~Texture2D();
	void Activate(Shader& shader, int textureUnit = 0);

private:
	int width, height, channels;
};

