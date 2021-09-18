#include "Graphics.h"

void Graphics::init()
{
	render_shader = Shader("voxelConeTracing/Shaders/render_vert.vs", "voxelConeTracing/Shaders/render_frag.fs");
	//voxelizaition_shader = Shader("voxelConeTracing/Shaders/voxel_vert.vs", "voxel/ConeTracing/Shaders/voxel_frag.fs", "voxel/ConeTracing/Shaders/voxel_geom.gs");
}

void Graphics::render(std::shared_ptr<Scene>& renderingScene, Camera& camera, RenderingMode renderMode)
{
	renderScene(renderingScene, camera);
}

void Graphics::renderScene(std::shared_ptr<Scene>& renderingScene, Camera& camera)
{
	//background
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