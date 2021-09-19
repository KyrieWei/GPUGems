#pragma once
#include <memory>
#include "../Scenes/Scene.h"
#include "../../tools/Shader.h"
#include "../../tools/standardModel.h"
#include "Texture3D.h"
#include "FBO.h"

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
	virtual void init(unsigned int viewportWidth, unsigned int viewportHeight);

	//rendering a scene
	virtual void render(std::shared_ptr<Scene>& renderingScene, Camera& camera, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderMode = RenderingMode::VOXEL_CONE_TRACING);

	//rendering
	bool shadows = true;
	bool indirectDiffuseLight = true;
	bool indirectSpecularLight = true;
	bool directLight = true;

	//voxelization
	bool automaticallyRegenerateMipmap = true;
	bool regenerateMipmapQueued = true;
	bool automaticallyVoxelize = true;
	bool voxelizationQueued = true;
	int voxelizationSparsity = 1;

	//shader
	Shader render_shader;
	Shader voxelizaition_shader;
	Shader worldPosition_shader;
	Shader voxelVisualization_shader;
private:

	void renderScene(std::shared_ptr<Scene>& renderingScene, Camera& camera, unsigned int viewportWidth, unsigned int viewportHeight);
	void renderQueue(std::vector<Model>& models);
	void setCameraInfo(Camera& camera, Shader& shader);
	void setLightInfo(std::shared_ptr<Scene>& renderingScene, Shader& shader);
	void setRenderingSettings(Shader& shader);
	void setGlobalConstants(Shader& shader, unsigned int viewportWidth, unsigned int viewportHeight);

	//voxelization
	GLuint voxelTextureSize = 64; // must be set to a power of 2
	Texture3D* voxelTexture = nullptr;
	void initVoxelization();
	void voxelize(std::shared_ptr<Scene>& renderingScene, bool clearVoxelizationFirst = true);

	//voxelization visualization
	void initVoxelVisualization(unsigned int viewportWidth, unsigned int viewportHeight);
	void renderVoxelVisualization(std::shared_ptr<Scene>& renderingScene, Camera& camera, unsigned int viewportWidth, unsigned int viewportHeight);
	
	FBO* vvfbo1, * vvfbo2;
	Model cubeModel;
	Quad quad;
};

