#pragma once
class Graphics
{
public:
	enum RenderingMode
	{
		VOXELIZATION_VISUALIZATION = 0,
		VOXEL_CONE_TRACING = 1
	};

	//initialize rendering
	virtual void init(unsigned int viewportWidth, unsigned int viewportHeight);

	//rendering a scene
	virtual void render(unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderMode = RenderingMode::VOXEL_CONE_TRACING);

	//rendering
	bool shadows = true;
	bool indirectDiffuseLight = true;
	bool indirectSpecularLight = true;
	bool directLight = true;

	//voxelization
	bool automaticallyRegenerateMipmap = true;

};

