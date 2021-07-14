#include "countlessGrass.h"

#include "../tools/Utility.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../tools/stb_image.h"

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == (GL_RGBA || GL_RED) ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == (GL_RGBA || GL_RED) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

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

    float vertices[] = {
        // positions         // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    //float* ground_vertex_data = new float[ground_vertex_num * stride];
    //unsigned int* ground_index = new unsigned int[ground_tri_num * 3];

    //generate_mesh("countlessGrass/assets/ground_mesh.txt", res_x, res_y, ground_width, ground_height);

    /*
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

    */
#pragma endregion

#pragma region grass

    Shader grass_shader("countlessGrass/shaders/grass_vert.vs", "countlessGrass/shaders/grass_frag.fs", "countlessGrass/shaders/grass_geom.gs");

    float points[] = {
     -0.8f, -0.9f, 0.0f,  // 左上
     -0.3f, -0.9f, 0.0f,  // 右上
      0.3f, -0.9f, 0.0f,  // 右下
      0.8f, -0.9f, 0.0f   // 左下
    };

    unsigned int grass_VAO, grass_VBO;

    glGenVertexArrays(1, &grass_VAO);
    glGenBuffers(1, &grass_VBO);
    
    glBindVertexArray(grass_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, grass_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    
    glBindVertexArray(0);


    unsigned int grass_mask_id = loadTexture("countlessGrass/assets/grass_mask.png");

    stbi_set_flip_vertically_on_load(true);
    unsigned int grass_texture_id = loadTexture("countlessGrass/assets/grass_texture.png");

    float grass_width = 0.1f, grass_height = 0.5f;

    grass_shader.use();
    grass_shader.setFloat("grass_width", grass_width);
    grass_shader.setFloat("grass_height", grass_height);

    grass_shader.setInt("grass_texture", 0);
    grass_shader.setInt("grass_mask", 1);

#pragma endregion


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //camera.setInitialStatus(glm::vec3(-5.5, 3.5, -22), glm::vec3(0.15, 0.9, 0.3), 780, -18);

    while (!glfwWindowShouldClose(window))
    { 
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //ground 
        ground_shader.use();

        glm::mat4 model = glm::mat4(1.0);
        ground_shader.setMat4("model", model);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ground_shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ground_shader.setMat4("view", view);

        glBindTexture(GL_TEXTURE_2D, grass_mask_id);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, ground_tri_num * 3, GL_UNSIGNED_INT, 0);

        //grass
        //grass_shader.use();
        //glm::mat4 model = glm::mat4(1.0);
        //grass_shader.setMat4("model", model);

        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //grass_shader.setMat4("projection", projection);

        //glm::mat4 view = camera.GetViewMatrix();
        //grass_shader.setMat4("view", view);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, grass_texture_id);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, grass_mask_id);

        //glBindVertexArray(grass_VAO);
        //glDrawArrays(GL_POINTS, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //glDeleteBuffers(1, &ground_VAO);
    //glDeleteBuffers(1, &ground_VBO);
    //glDeleteBuffers(1, &ground_EBO);

    glDeleteBuffers(1, &grass_VAO);
    glDeleteBuffers(1, &grass_VBO);

    glfwTerminate();

    //delete[] ground_vertex_data;
    //delete[] ground_index;
}

