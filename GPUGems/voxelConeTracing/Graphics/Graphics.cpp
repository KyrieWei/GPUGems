#include "Graphics.h"

void Graphics::init(unsigned int viewportWidth, unsigned int viewportHeight)
{
	render_shader = Shader("voxelConeTracing/Shaders/render_vert.vs", "voxelConeTracing/Shaders/render_frag.fs");
	voxelizaition_shader = Shader("voxelConeTracing/Shaders/voxel_vert.vs", "voxelConeTracing/Shaders/voxel_frag.fs", "voxelConeTracing/Shaders/voxel_geom.gs");

	initVoxelization();
	initVoxelVisualization(viewportWidth, viewportHeight);
}

void Graphics::render(std::shared_ptr<Scene>& renderingScene, Camera& camera, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderMode)
{
	bool voxelizeNow = true;
	if (voxelizeNow)
	{
		voxelize(renderingScene, true);
	}

	switch (renderMode)
	{
	case Graphics::VOXELIZATION_VISUALIZATION:
		renderVoxelVisualization(renderingScene, camera, viewportWidth, viewportHeight);
		break;
	case Graphics::VOXEL_CONE_TRACING:
		renderScene(renderingScene, camera, viewportWidth, viewportHeight);
		break;
	default:
		break;
	}
}

void Graphics::renderScene(std::shared_ptr<Scene>& renderingScene, Camera& camera, unsigned int viewportWidth, unsigned int viewportHeight)
{
	//background

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	render_shader.use();

	glViewport(0, 0, viewportWidth, viewportHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GL settings
	glEnable(GL_DEPTH_TEST);

	setCameraInfo(camera, render_shader);
	setLightInfo(renderingScene, render_shader);
	setRenderingSettings(render_shader);

	renderQueue(renderingScene->models);
}

void Graphics::renderQueue(std::vector<Model>& models)
{
	for (auto& elem : models)
	{
		elem.Draw(render_shader);
	}
}

//Voxelization
void Graphics::initVoxelization()
{
	const std::vector<GLfloat> texture3D(4 * voxelTextureSize * voxelTextureSize * voxelTextureSize, 0.0f);
	voxelTexture = new Texture3D(texture3D, voxelTextureSize, voxelTextureSize, voxelTextureSize, true);
}

void Graphics::voxelize(std::shared_ptr<Scene>& renderingScene, bool clearVoxelizationFirst)
{
	if (clearVoxelizationFirst)
	{
		GLfloat clearColor[4] = { 0, 0, 0, 0 };
		voxelTexture->Clear(clearColor);
	}

	voxelizaition_shader.use();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//settings
	glViewport(0, 0, voxelTextureSize, voxelTextureSize);

	voxelTexture->Activate(voxelizaition_shader, "texture3D", 0);
	glBindImageTexture(0, voxelTexture->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	//Lighting
	setLightInfo(renderingScene, voxelizaition_shader);

	renderQueue(renderingScene->models);
}

//Voxelization visualization
void Graphics::initVoxelVisualization(unsigned int viewportWidth, unsigned int viewportHeight)
{
	worldPosition_shader = Shader("voxelConeTracing/Shaders/worldposition_vert.vs", "voxelConeTracing/Shaders/worldposition_frag.fs");
	voxelVisualization_shader = Shader("voxelConeTracing/Shaders/voxel_visualization_vert.vs", "voxelConeTracing/Shaders/voxel_visualization_frag.fs");

	//FBO
	vvfbo1 = new FBO(viewportWidth, viewportHeight);
	vvfbo2 = new FBO(viewportWidth, viewportHeight);

	cubeModel = Model("voxelConeTracing/assets/models/cube.obj");
	quad = Quad();
}

void Graphics::renderVoxelVisualization(std::shared_ptr<Scene>& renderingScene, Camera& camera, unsigned int viewportWidth, unsigned int viewportHeight)
{
	worldPosition_shader.use();

	setCameraInfo(camera, worldPosition_shader);

	//settings
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Back
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, vvfbo1->frameBuffer);
	glViewport(0, 0, vvfbo1->width, vvfbo1->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cubeModel.Draw(worldPosition_shader);

	//Front
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, vvfbo2->frameBuffer);
	glViewport(0, 0, vvfbo2->width, vvfbo2->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cubeModel.Draw(worldPosition_shader);

	//render 3d texture to screen
	voxelVisualization_shader.use();
	setCameraInfo(camera, voxelVisualization_shader);
	glBindFramebuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//settngs
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	vvfbo1->ActivateAsTexture(voxelVisualization_shader, "textureBack", 0);
	vvfbo2->ActivateAsTexture(voxelVisualization_shader, "textureFront", 1);
	voxelTexture->Activate(voxelVisualization_shader, "texture3D", 2);

	//render
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad.Draw(voxelVisualization_shader);
}

void Graphics::setCameraInfo(Camera& camera, Shader& shader)
{
	shader.use();
	shader.setMat4("model", glm::mat4(1.0));
	shader.setMat4("view", camera.GetViewMatrix());
	shader.setMat4("projection", camera.GetProjectMatrix());
	shader.setVec3("cameraPosition", camera.Position);
}

void Graphics::setLightInfo(std::shared_ptr<Scene>& renderingScene, Shader& shader)
{
	for (unsigned int i = 0; i < renderingScene->pointLights.size(); i++)
		renderingScene->pointLights[i].Upload(shader, i);

	shader.setInt("numberOfLights", renderingScene->pointLights.size());
}

void Graphics::setRenderingSettings(Shader& shader)
{
	shader.use();
	shader.setBool("settings.shadows", shadows);
	shader.setBool("settings.indirectDiffuseLight", indirectDiffuseLight);
	shader.setBool("settings.indirectSpecularLight", indirectSpecularLight);
	shader.setBool("settings.directLight", directLight);
}

void Graphics::setGlobalConstants(Shader& shader, unsigned int viewportWidth, unsigned int viewportHeight)
{
	shader.use();
	
}