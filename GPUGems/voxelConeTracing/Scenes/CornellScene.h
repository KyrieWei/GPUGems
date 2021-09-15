#pragma once
#include <vector>
#include "Scene.h"

class CornellScene : public Scene
{
public:
	void update();
	void init(unsigned int viewportWidth, unsigned int viewportHeight);
	~CornellScene();
};

