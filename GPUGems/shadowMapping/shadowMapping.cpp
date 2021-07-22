#include "shadowMapping.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../tools/stb_image.h"

void show_shadow_mapping(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

#pragma region floor

    GLint floor_vertex_stride = 5;

    float floor[4 * 5] =
    {
        -5.0f, 0.0f, -5.0f, 0.0f, 0.0f,
         5.0f, 0.0f, -5.0f, 5.0f, 0.0f,
         5.0f, 0.0f,  5.0f, 5.0f, 5.0f,
        -5.0f, 0.0f,  5.0f, 0.0f, 5.0f
    };

    unsigned int floor_index[6] =
    {
        0, 1, 2,
        0, 2, 3
    };

    GLuint floor_VAO, floor_VBO, floor_EBO;
    glGenVertexArrays(1, &floor_VAO);
    glGenBuffers(1, &floor_VBO);
    glGenBuffers(1, &floor_EBO);

    glBindVertexArray(floor_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, floor_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_index), floor_index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floor_vertex_stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, floor_vertex_stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint floor_texture;
    glGenTextures(1, &floor_texture);
    glBindTexture(GL_TEXTURE_2D, floor_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint floor_texture_width, floor_texture_height, floor_texture_channel;
    GLubyte* floor_texture_data = stbi_load("shadowMapping/assets/wood.png", &floor_texture_width, &floor_texture_height, &floor_texture_channel, 0);

    if (floor_texture_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floor_texture_width, floor_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, floor_texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load floor texture" << std::endl;
    }

    stbi_image_free(floor_texture_data);

    Shader floor_shader("shadowMapping/shaders/floor_vert.vs", "shadowMapping/shaders/floor_frag.fs");

    floor_shader.use();
    floor_shader.setInt("floor_texture", 0);

#pragma endregion

#pragma region box



#pragma endregion


    glEnable(GL_DEPTH_TEST);

    camera.setInitialStatus(glm::vec3(4.0f, 4.0f, 7.0f), glm::vec3(-0.3f, 0.8f, -0.5f), -120, -32);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        floor_shader.use();
        
        glm::mat4 model = glm::mat4(1.0);
        floor_shader.setMat4("model", model);

        glm::mat4 view = camera.GetViewMatrix();
        floor_shader.setMat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        floor_shader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_texture);

        glBindVertexArray(floor_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &floor_VAO);
    glDeleteBuffers(1, &floor_VBO);
    glDeleteBuffers(1, &floor_EBO);

    glfwTerminate();
}