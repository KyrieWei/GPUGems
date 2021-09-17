#pragma once
#include <memory>
#include "../Scenes/Scene.h"
#include "../../tools/Shader.h"

class Graphics
{
public:
	enum RenderingMode
	{
		VOXELIZATION_VISUALIZATION = 0,
		VOXEL_CONE_TRACING = 1
	};

	Graphics() {}

	//initialize rendering
	virtual void init();

	//rendering a scene
	virtual void render(std::shared_ptr<Scene>& renderingScene, Camera& camera, RenderingMode renderMode = RenderingMode::VOXEL_CONE_TRACING);

	//rendering
	bool shadows = true;
	bool indirectDiffuseLight = true;
	bool indirectSpecularLight = true;
	bool directLight = true;

	//voxelization
	bool automaticallyRegenerateMipmap = true;

	//shader
	Shader shader;

private:

	

	void renderScene(std::shared_ptr<Scene>& renderingScene, Camera& camera);
	void renderQueue(std::vector<Model>& models);
	void setCameraInfo(Camera& camera, Shader& shader);
	void setLightInfo(std::shared_ptr<Scene>& renderingScene, Shader& shader);

};

