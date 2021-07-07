//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#include "effectiveWater.h"
#include "../tools/Maths.h"
#include "../tools/Utility.h"

const int num_func = 3;


void show_effective_water(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    Shader shader("effectiveWater/shaders/vert.vs", "effectiveWater/shaders/frag.fs");

    int res_x = 100, res_y = 100;
    int vertex_num = (res_x + 1) * (res_y + 1);
    int tri_num = res_x * res_y * 2;

    //generate_mesh("water_mesh.txt", res_x, res_y);

    float* water_mesh_vertex = new float[vertex_num * 3];
    unsigned int* water_mesh_index = new unsigned int[tri_num * 3];

    load_mesh("effectiveWater/assets/water_mesh.txt", water_mesh_vertex, water_mesh_index);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_num * 3, water_mesh_vertex, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tri_num * 3, water_mesh_index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    int sigma = 10;

    //wave.xy = direction
    //wave.z = amplitude
    //wave.w = wavelength

    glm::vec4 wave[num_func];
    float speed[num_func];
    
    for (int i = 0; i < num_func; i++)
    {   
        std::vector<float> dir = random_dir(-30, -60);
        wave[i].x = dir[0];
        wave[i].y = dir[1];
        wave[i].z = RandT<float>(0.02, 0.05);
        wave[i].w = RandT<float>(0.6, 0.7);
        
        speed[i] = RandT<float>(0.1, 0.3);
    }

    float Q = 0.4;

    unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader.ID, "params");
    glUniformBlockBinding(shader.ID, uniformBlockIndex, 0);

    unsigned int uboBlock;
    glGenBuffers(1, &uboBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, uboBlock);
    glBufferData(GL_UNIFORM_BUFFER, num_func * sizeof(glm::vec4) + num_func * sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboBlock, 0, num_func * sizeof(glm::vec4) + num_func * sizeof(float));

    glBindBuffer(GL_UNIFORM_BUFFER ,uboBlock);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, num_func * sizeof(glm::vec4), wave);
    glBufferSubData(GL_UNIFORM_BUFFER, num_func * sizeof(glm::vec4), num_func * sizeof(float), speed);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    shader.use();
    shader.setFloat("k", 2.5);
    shader.setFloat("Q", Q);

    camera.setInitialStatus(glm::vec3(-0.6, 1.0, 2.3), glm::vec3(0.125, 0.9, -0.4), -72, -24);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        shader.setFloat("_Time", currentFrame);

        glm::mat4 model = glm::mat4(1.0);
        shader.setMat4("model", model);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, tri_num * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] water_mesh_vertex;
    delete[] water_mesh_index;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
}


