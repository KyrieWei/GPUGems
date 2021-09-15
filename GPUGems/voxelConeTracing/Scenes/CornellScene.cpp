#include "CornellScene.h"


void CornellScene::init(unsigned int viewportWidth, unsigned int viewportHeight)
{
	//initialize camera information
	camera.setPerspectiveProject((float)viewportWidth / (float)viewportHeight, 0.1f, 100.0f);

	Model cornellBox("voxelConeTracing/assets/models/cornell.obj");
	models.push_back(cornellBox);

}

void CornellScene::update()
{

}

CornellScene::~CornellScene()
{

}