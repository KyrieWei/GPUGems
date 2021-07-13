#include "countlessGrass.h"
#include "../tools/Utility.h"

void show_countless_grass(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

#pragma region grass_ground

    Shader ground_shader("countlessGrass/shaders/ground_vert.vs", "countlessGrass/shaders/ground_frag.fs");

    int res_x = 400, res_y = 400, ground_width = 40, ground_height = 40;
    int ground_vertex_num = (res_x + 1) * (res_y + 1);
    int ground_tri_num = res_x * res_y * 2;

    int stride = 5;
    
    float* ground_vertex_data = new float[ground_vertex_num * stride];
    unsigned int* ground_index = new unsigned int[ground_tri_num * 3];

    //generate_mesh("countlessGrass/assets/ground_mesh.txt", res_x, res_y, ground_width, ground_height);

    load_mesh("countlessGrass/assets/ground_mesh.txt", ground_vertex_data, ground_index);

    unsigned int ground_VAO, ground_VBO, ground_EBO;

    glGenVertexArrays(1, &ground_VAO);
    glGenBuffers(1, &ground_VBO);
    glGenBuffers(1, &ground_EBO);

    glBindVertexArray(ground_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ground_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ground_vertex_num * stride, ground_vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ground_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ground_tri_num * 3, ground_index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#pragma endregion

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    camera.setInitialStatus(glm::vec3(-5.5, 3.5, -22), glm::vec3(0.15, 0.9, 0.3), 780, -18);

    while (!glfwWindowShouldClose(window))
    { 
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ground_shader.use();

        glm::mat4 model = glm::mat4(1.0);
        ground_shader.setMat4("model", model);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ground_shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ground_shader.setMat4("view", view);

        glBindVertexArray(ground_VAO);
        glDrawElements(GL_TRIANGLES, ground_tri_num * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteBuffers(1, &ground_VAO);
    glDeleteBuffers(1, &ground_VBO);
    glDeleteBuffers(1, &ground_EBO);

    glfwTerminate();

    delete[] ground_vertex_data;
    delete[] ground_index;
}