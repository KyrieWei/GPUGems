#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include "../../tools/Shader.h"

class FBO
{
public:
	GLuint width, height, frameBuffer, textureColorBuffer, attachment, rbo;
	void ActivateAsTexture(Shader& shader, const std::string glSamplerName, const int textureUnit = GL_TEXTURE0);

	FBO(GLuint w, GLuint h, GLenum magFilter = GL_NEAREST, GLenum minFilter = GL_NEAREST, GLint internalFormat = GL_RGB16F, GLint format = GL_FLOAT, GLint wrap = GL_REPEAT);
	~FBO();

private:
	GLuint generateAttachment(GLuint w, GLuint h, GLboolean depth, GLboolean stencil, GLenum magFilter, GLenum minFilter, GLenum wrap);
};

