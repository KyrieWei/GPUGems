#include "CornellScene.h"

void CornellScene::init(unsigned int viewportWidth, unsigned int viewportHeight)
{
	//initialize camera information
	camera.setPerspectiveProject((float)viewportWidth / (float)viewportHeight, 0.1f, 100.0f);

	Model cornellBox("voxelConeTracing/assets/models/cornell.obj");
	Model lightSphere("voxelConeTracing/assets/models/sphere.obj");
	//Model dragon("voxelConeTracing/assets/models/dragon.obj");

	//Cornel box
	cornellBox.meshes[0].materialSetting = MaterialSetting(glm::vec3(0.35f, 0.38f, 1.0f));   //Green wall
	cornellBox.meshes[1].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //Floor
	cornellBox.meshes[2].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //Roof
	cornellBox.meshes[3].materialSetting = MaterialSetting(glm::vec3(1.0f, 0.26f, 0.27f));   //Red wall
	cornellBox.meshes[4].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //white wall
	cornellBox.meshes[5].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //left box
	cornellBox.meshes[6].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //right box

	cornellBox.useMaterialSetting = true;

	//light sphere
	lightSphere.meshes[0].materialSetting = MaterialSetting(glm::vec3(1.0f, 1.0f, 1.0f), 8.0f, 0.0f, 0.0f);
	lightSphere.useMaterialSetting = true;

	models.push_back(cornellBox);
	//models.push_back(dragon);
	models.push_back(lightSphere);
	
	PointLight p;
	p.color = normalize(glm::vec3(1.4f, 0.9f, 0.35f));
	pointLights.push_back(p);
	
}

void CornellScene::update()
{
	//light sphere
	int index = models.size() - 1;
	models[index].transform.position = glm::vec3(0, 0.5, 0.1);
	models[index].transform.position.x *= 4.5f;
	models[index].transform.position.z *= 4.5f;

	models[index].transform.scale = glm::vec3(0.05f);
	models[index].transform.updateTransformMatrix();

	pointLights[0].position = models[1].transform.position;

}

CornellScene::~CornellScene()
{

}