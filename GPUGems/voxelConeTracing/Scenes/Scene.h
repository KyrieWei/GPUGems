#pragma once
#include "../../tools/Camera.h"
#include "../Graphics/PointLight.h"

class MeshRenderer;

class Scene
{
public:
	Camera* renderingCamera;

	std::vector<MeshRenderer*> renderers;
	std::vector<PointLight> pointLights;

	virtual void update() = 0;
	virtual void init(unsigned int viewportWidth, unsigned int viewportHeight) = 0;

	Scene() {}
};

