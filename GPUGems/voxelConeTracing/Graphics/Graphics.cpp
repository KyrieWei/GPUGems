#include "Graphics.h"

void Graphics::init()
{
	shader = Shader("voxelConeTracing/Shaders/render_vert.vs", "voxelConeTracing/Shaders/render_frag.fs");
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


	setCameraInfo(camera, shader);
	
	renderQueue(renderingScene->models);
}

void Graphics::renderQueue(std::vector<Model>& models)
{
	for (auto& elem : models)
	{
		elem.Draw(shader);
	}
}

void Graphics::setCameraInfo(Camera& camera, Shader& shader)
{
	shader.use();
	shader.setMat4("model", glm::mat4(1.0));
	shader.setMat4("view", camera.GetViewMatrix());
	shader.setMat4("projection", camera.GetProjectMatrix());
}