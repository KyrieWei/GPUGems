#include "TestForAlgorithm.h"
#include "../tools/Maths.h"

struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

void UniformSamplingSphere(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;


    std::vector<Vector3f> sampling_points;

    int sqrt_n_samples = 50;

    for (int i = 0; i < sqrt_n_samples; i++)
    {
        for (int j = 0; j < sqrt_n_samples; j++)
        {
            
            float u = RandT<float>(0, 1);
            float v = RandT<float>(0, 1);

            float z = 1 - 2 * u;
            float r = std::sqrt(std::max(0.0f, 1.0f - z * z));
            float phi = 2 * M_PI * v;

            sampling_points.push_back(Vector3f(r * std::cos(phi), r * std::sin(phi), z));
            
            
            //float theta = RandT<float>(0, M_PI);
            //float phi = RandT<float>(0, M_PI);

            //sampling_points.push_back(Vector3f(std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi)));
        }
    }

    unsigned int vertex_VAO, vertex_VBO, vertex_EBO;

    glGenVertexArrays(1, &vertex_VAO);
    glGenBuffers(1, &vertex_VBO);
    glGenBuffers(1, &vertex_EBO);

    glBindVertexArray(vertex_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * sqrt_n_samples * sqrt_n_samples, &sampling_points[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glBindVertexArray(0);

    Shader shader("testForAlgorithm/shaders/vert.vs", "testForAlgorithm/shaders/frag.fs");

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        
        glBindVertexArray(vertex_VAO);
        glPointSize(2.0);
        glDrawArrays(GL_POINTS, 0, sqrt_n_samples * sqrt_n_samples);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
}


void show_test_for_algorithm(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{
    UniformSamplingSphere(window, camera, SCR_WIDTH, SCR_HEIGHT, processInput);
}