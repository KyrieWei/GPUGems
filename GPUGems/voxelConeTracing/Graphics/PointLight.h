#pragma once
#include "../../tools/Shader.h"

class PointLight
{
public:
	glm::vec3 position, color;
	PointLight(glm::vec3 _position = { 0, 0, 0 }, glm::vec3 _color = { 1, 1, 1 }) : position(_position), color(_color) {}
	void Upload(Shader& shader, GLuint index) const
	{
		shader.setVec3("pointLights[" + std::to_string(index) + "].position", position);
		shader.setVec3("pointLights[" + std::to_string(index) + "].color", color);
	}
};

