#include "waterCaustic.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"
#include "../tools/Utility.h"

void show_water_caustic(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

#pragma region water_ground

    float water_ground[] =
    {
        //  ---position---      -texture-
          2.0f, 0.0f,  2.0f,   1.0f, 1.0f,
          2.0f, 0.0f, -2.0f,   1.0f, 0.0f,
         -2.0f, 0.0f, -2.0f,   0.0f, 0.0f,
         -2.0f, 0.0f,  2.0f,   0.0f, 1.0f
    };

    unsigned int water_ground_index[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    int water_ground_vertex_num = 12;
    int water_ground_index_num = 6;

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    //vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(water_ground), water_ground, GL_STATIC_DRAW);

    //index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(water_ground_index), water_ground_index, GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //set wrap and filter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("waterCaustic/assets/MURO_2.tga", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texutre" << std::endl;
    }
    stbi_image_free(data);

#pragma endregion

#pragma region water_surface

    //generate_mesh("waterCaustic/assets/water_surface.txt", 100, 100, 4, 4);

    int res_x = 100, res_y = 100;
    int vertex_num = (res_x + 1) * (res_y + 1);
    int tri_num = res_x * res_y * 2;

    float* water_surface_vertex = new float[vertex_num * 3];
    unsigned int* water_surface_index = new unsigned int[tri_num * 3];

    load_mesh("waterCaustic/assets/water_surface.txt", water_surface_vertex, water_surface_index);

    unsigned int surface_VAO, surface_VBO, surface_EBO;
    glGenVertexArrays(1, &surface_VAO);
    glGenBuffers(1, &surface_VBO);
    glGenBuffers(1, &surface_EBO);

    glBindVertexArray(surface_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, surface_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_num * 3, water_surface_vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tri_num * 3, water_surface_index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#pragma endregion


 
    //MSAA
    glEnable(GL_MULTISAMPLE);

    Shader shader("waterCaustic/shaders/vert.vs", "waterCaustic/shaders/frag.fs");

    camera.setInitialStatus(glm::vec3(0, 2.7, 5.5), glm::vec3(-0.02, 0.9, 0.4), -93, -23);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0);
        shader.setMat4("model", model);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, water_ground_index_num, GL_UNSIGNED_INT, 0);

        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        shader.setMat4("model", model);

        glBindVertexArray(surface_VAO);
        glDrawElements(GL_TRIANGLES, tri_num * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
}