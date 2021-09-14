#pragma once
#include <vector>

class Shape;

class CornellScene
{
public:
	void update();
	void init(unsigned int viewportWidth, unsigned int viewportHeight);
	~CornellScene();
private:
	std::vector<Shape*> shapes;
};

