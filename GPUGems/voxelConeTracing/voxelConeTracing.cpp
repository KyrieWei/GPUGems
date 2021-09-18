#include "voxelConeTracing.h"
#include "Scenes/CornellScene.h"


void voxelConeTracing::init(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT)
{

    graphics.init();

    CornellScene cornellScene;
    cornellScene.init(SCR_WIDTH, SCR_HEIGHT);
    
    scene = std::make_shared<CornellScene>(cornellScene);
}

void voxelConeTracing::run(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    camera.setPerspectiveProject(float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);

        scene->update();
        graphics.render(scene, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}