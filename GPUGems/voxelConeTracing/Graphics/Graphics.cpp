#include "Graphics.h"
#include "../../tools/Maths.h"

void Graphics::init(unsigned int viewportWidth, unsigned int viewportHeight, Scene& renderingScene)
{
	render_shader = Shader("voxelConeTracing/Shaders/render_vert.vs", "voxelConeTracing/Shaders/render_frag.fs");
	voxelization_shader = Shader("voxelConeTracing/Shaders/voxel_vert.vs", "voxelConeTracing/Shaders/voxel_frag.fs", "voxelConeTracing/Shaders/voxel_geom.gs");
	worldPosition_shader = Shader("voxelConeTracing/Shaders/worldposition_vert.vs", "voxelConeTracing/Shaders/worldposition_frag.fs");
	//voxelizaition_shader = Shader("voxelConeTracing/Shaders/voxel_vert.vs", "voxelConeTracing/Shaders/voxel_frag.fs");

	initVoxelization();
	initVoxelVisualization(viewportWidth, viewportHeight);

	//voxelize(renderingScene.models[1], 0.01f, ret);

	//std::cout << ret.size() << std::endl;
	//
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ret.size(), &ret[0], GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//glBindVertexArray(0);

}

void Graphics::render(std::shared_ptr<Scene>& renderingScene, Camera& camera, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderMode)
{
	bool voxelizeNow = true;
	if (voxelizeNow)
	{
		voxelize(renderingScene, camera, true);
	}

	/*
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	voxelize(renderingScene->models[1], 0.01f, ret);

	worldPosition_shader.use();
	setCameraInfo(camera, worldPosition_shader);
	
	glBindVertexArray(VAO);
	glPointSize(0.01f);
	glDrawArrays(GL_POINTS, 0, ret.size());
	*/

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

	glViewport(0, 0, voxelTextureSize, voxelTextureSize);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GL settings
	glEnable(GL_DEPTH_TEST);

	setCameraInfo(camera, render_shader);
	setLightInfo(renderingScene, render_shader);
	setRenderingSettings(render_shader);

	renderQueue(renderingScene->models, render_shader);
}

void Graphics::renderQueue(std::vector<Model>& models, Shader& shader)
{
	for (auto& elem : models)
	{
		elem.Draw(shader);
	}
}

//Voxelization
void Graphics::initVoxelization()
{
	const std::vector<GLfloat> texture3D(4 * voxelTextureSize * voxelTextureSize * voxelTextureSize, 0.0f);
	voxelTexture = new Texture3D(texture3D, voxelTextureSize, voxelTextureSize, voxelTextureSize, true);
}

void Graphics::voxelize(std::shared_ptr<Scene>& renderingScene, Camera& camera, bool clearVoxelizationFirst)
{
	if (clearVoxelizationFirst)
	{
		GLfloat clearColor[4] = { 0, 0, 0, 0 };
		voxelTexture->Clear(clearColor);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	voxelization_shader.use();

	//settings
	glViewport(0, 0, voxelTextureSize, voxelTextureSize);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	voxelTexture->Activate(voxelization_shader, "texture3D", 0);
	glBindImageTexture(0, voxelTexture->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	//Lighting
	setCameraInfo(camera, voxelization_shader);
	setLightInfo(renderingScene, voxelization_shader);

	renderQueue(renderingScene->models, voxelization_shader);
}

void Graphics::voxelize(Model& model, float step, std::vector<glm::vec3>& ret)
{
	ret.clear();

	Shader voxelShader = Shader("voxelConeTracing/Shaders/voxelize_vert.vs", "voxelConeTracing/Shaders/voxelize_frag.fs");

	glm::vec3 min, max;
	glm::ivec3 resolution;

	model.getAABB(min, max);
	
	glm::vec3 range(max.x - min.x, max.y - min.y, max.z - min.z);
	resolution.x = static_cast<int>(range.x / step) + 1;
	resolution.y = static_cast<int>(range.y / step) + 1;
	resolution.z = static_cast<int>(range.z / step) + 1;
	int length = resolution.x * resolution.y * resolution.z;

	float offset = 0.2f;
	glm::vec3 cameraPosZ, cameraPosX, cameraPosY;

	cameraPosZ.x = (min.x + max.x) * 0.5f;
	cameraPosZ.y = (min.y + max.y) * 0.5f;
	cameraPosZ.z = max.z + offset;

	Camera cameraZ(cameraPosZ);
	cameraZ.setOrthographicProject(-range.x * 0.51, range.x * 0.51, -range.y * 0.51, range.y * 0.51, 0.1, range.z * 1.2f + offset);

	cameraPosX.x = max.x + offset;
	cameraPosX.y = (min.y + max.y) * 0.5f;
	cameraPosX.z = (min.z + max.z) * 0.5f;

	Camera cameraX(cameraPosX);

	cameraPosY.x = (min.x + max.x) * 0.5f;
	cameraPosY.y = max.y + offset;
	cameraPosY.z = (min.z + max.z) * 0.5f;

	Camera cameraY(cameraPosY);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//generate ssbo.
	glGenBuffers(1, &m_cntBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_cntBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, length * sizeof(int), nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_cntBuffer);

	//bind shader adn ssbo
	voxelShader.use();
	voxelShader.setVec3("boxMin", min);
	voxelShader.setFloat("step", step);
	voxelShader.setVec3("resolution", resolution);
	voxelShader.setMat4("view", cameraZ.GetViewMatrix());
	voxelShader.setMat4("project", cameraZ.GetProjectMatrix());
	
	int* writePtr = reinterpret_cast<int*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
	for (int i = 0; i < length; i++)
		writePtr[i] = 0;

	if (!glUnmapBuffer(GL_SHADER_STORAGE_BUFFER))
		std::cout << "unMap error\n" << std::endl;

	//draw and count
	model.Draw(voxelShader);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_cntBuffer);

	int* readPtr = reinterpret_cast<int*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));

	if (readPtr != nullptr)
	{
		for (int i = 0; i < length; i++)
		{
			if (*(readPtr + i) != 0)
			{
				int iy = i / (resolution.x * resolution.z);
				int iz = (i - iy * resolution.x * resolution.z) / (resolution.x);
				int ix = i - iy * resolution.x * resolution.z - iz * resolution.x;
				glm::vec3 elem = min + glm::vec3(ix * step, iy * step, iz * step);
				ret.push_back(elem);
			}
		}
	}
	else
		std::cout << "nullptr error\n";

	//for (int i = 0; i < ret.size(); i++)
	//	std::cout << ret[i].x << " " << ret[i].y << " " << ret[i].z << std::endl;

	glUnmapBuffer(m_cntBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glDeleteBuffers(1, &m_cntBuffer);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.3, 0.3, 1.0);

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

void Graphics::endFrame()
{
	
}