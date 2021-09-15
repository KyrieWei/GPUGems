#pragma once
#include "../../tools/Camera.h"
#include "../Graphics/PointLight.h"
#include "../../tools/Model.h"

class MeshRenderer;

class Scene
{
public:
	Camera camera;

	virtual void update() = 0;
	virtual void init(unsigned int viewportWidth, unsigned int viewportHeight) = 0;

	Scene() {}

public:
	std::vector<Model> models;
};

