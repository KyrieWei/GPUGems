#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../tools/Shader.h"
#include "../tools/Camera.h"
#include "Graphics/Graphics.h"
#include "Scenes/Scene.h"

typedef void (*processInputCallback)(GLFWwindow* window, float deltaTime);

class voxelConeTracing
{
public:
	voxelConeTracing() {}
	void init(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
	void run(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput);

private:
	std::shared_ptr<Scene> scene;
	Graphics graphics;

};

