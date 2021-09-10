#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../tools/Shader.h"
#include "../tools/Camera.h"

typedef void (*processInputCallback)(GLFWwindow* window, float deltaTime);

void show_test_for_algorithm(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput);

