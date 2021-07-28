#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "tools/Camera.h"
#include "tools/Shader.h"

#include "effectiveWater/effectiveWater.h"
#include "waterCaustic/waterCaustic.h"
#include "countlessGrass/countlessGrass.h"
#include "shadowMapping/shadowMapping.h"
#include "omnidirectionalShadow/omnidirectionalShadow.h"
#include "toneMapping/toneMapping.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float M_PI = 3.141592657f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 16);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GPUGems", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 0 : effective water
    // 1 : water caustic
    // 2 : countless grass
    // 3 : shadow mapping
    // 4 : omnidirectional shadow
    // 5 : tone mapping

    int demo = 5;

    switch (demo)
    {
    case 0:
        glfwSetWindowTitle(window, "effective water");
        show_effective_water(window, camera, SCR_WIDTH, SCR_HEIGHT, processInput);
        break;
    case 1:
        glfwSetWindowTitle(window, "water caustic");
        show_water_caustic(window, camera, SCR_WIDTH, SCR_HEIGHT, processInput);
        break;
    case 2:
        glfwSetWindowTitle(window, "countless grass");
        show_countless_grass(window, camera, SCR_WIDTH, SCR_HEIGHT, processInput);
    case 3:
        glfwSetWindowTitle(window, "shadow mapping");
        show_shadow_mapping(window, camera, SCR_WIDTH, SCR_HEIGHT, processInput);
    case 4:
        glfwSetWindowTitle(window, "omnidirectional shadow");
        show_omnidirectional_shadow(window, camera, SCR_WIDTH, SCR_HEIGHT, processInput);
    case 5:
        glfwSetWindowTitle(window, "tone mapping");
        show_tone_mapping(window, camera, SCR_WIDTH, SCR_HEIGHT, processInput);
    default:
        break;
    }

    return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


