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

    int res_x = 400, res_z = 400, ground_x = 80, ground_y = 2, ground_z = 80;
    int ground_vertex_num = (res_x + 1) * (res_z + 1);
    int ground_tri_num = res_x * res_z * 2;

    int stride = 5;
    
    float* ground_vertex_data = new float[ground_vertex_num * stride];
    unsigned int* ground_index = new unsigned int[ground_tri_num * 3];

    //int height_map_width, height_map_height, height_map_channel;
    //unsigned char* height_map_data = stbi_load("countlessGrass/assets/Heightmap.png", &height_map_width, &height_map_height, &height_map_channel, 0);
    //if (!height_map_data)
    //    std::cout << "failed to load height map" << std::endl;
    //
    //generate_mesh_with_heightmap("countlessGrass/assets/ground_mesh.txt", res_x, res_z, ground_x, ground_y, ground_z, height_map_data, height_map_width, height_map_height, height_map_channel);
    //stbi_image_free(height_map_data);

    
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

    unsigned int ground_texture;
    glGenTextures(1, &ground_texture);
    glBindTexture(GL_TEXTURE_2D, ground_texture);

    //set wrap and filter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int ground_texture_width, ground_texture_height, ground_texture_channel;
    unsigned char* ground_texture_data = stbi_load("countlessGrass/assets/ground.jpg", &ground_texture_width, &ground_texture_height, &ground_texture_channel, 0);
    if (ground_texture_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ground_texture_width, ground_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, ground_texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load ground texture" << std::endl;
    }

    stbi_image_free(ground_texture_data);
    
#pragma endregion

#pragma region grass

    Shader grass_shader("countlessGrass/shaders/grass_vert.vs", "countlessGrass/shaders/grass_frag.fs", "countlessGrass/shaders/grass_geom.gs");

    stbi_set_flip_vertically_on_load(true);

    unsigned int grass_mask_texture;
    glGenTextures(1, &grass_mask_texture);
    glBindTexture(GL_TEXTURE_2D, grass_mask_texture);

    //set wrap and filter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int grass_mask_width, grass_mask_height, grass_mask_channel;
    unsigned char* grass_mask_texture_data = stbi_load("countlessGrass/assets/grassBladeAlpha.png", &grass_mask_width, &grass_mask_height, &grass_mask_channel, 0);
    if (grass_mask_texture_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grass_mask_width, grass_mask_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_mask_texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load grass mask texture" << std::endl;
    }
    
    stbi_image_free(grass_mask_texture_data);

    unsigned int grass_texture;
    glGenTextures(1, &grass_texture);
    glBindTexture(GL_TEXTURE_2D, grass_texture);

    //set wrap and filter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int grass_texture_width, grass_texture_height, grass_texture_channel;
    unsigned char* grass_texture_data = stbi_load("countlessGrass/assets/grass_texture.png", &grass_texture_width, &grass_texture_height, &grass_texture_channel, 0);
    if (grass_texture_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grass_texture_width, grass_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load grass texture" << std::endl;
    }

    stbi_image_free(grass_texture_data);

    float grass_width = 0.06f, grass_height = 0.4f;

    grass_shader.use();
    grass_shader.setFloat("grass_width", grass_width);
    grass_shader.setFloat("grass_height", grass_height);
    grass_shader.setFloat("mesh_unit_length_x", ground_x / (float)res_x);
    grass_shader.setFloat("mesh_unit_length_z", ground_z / (float)res_z);

    grass_shader.setInt("grass_texture", 0);
    grass_shader.setInt("grass_mask", 1);
    grass_shader.setVec2("wind_dir", glm::normalize(glm::vec2(1.0, 1.0)));

    ground_shader.use();
    ground_shader.setInt("ground_texture", 0);

#pragma endregion


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    camera.setInitialStatus(glm::vec3(-5.5, 10.5, -22), glm::vec3(0.15, 0.9, 0.3), 780, -18);

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ground_texture);

        glBindVertexArray(ground_VAO);
        glDrawElements(GL_TRIANGLES, ground_tri_num * 3, GL_UNSIGNED_INT, 0);

        //grass
        grass_shader.use();
        //glm::mat4 model = glm::mat4(1.0);
        grass_shader.setMat4("model", model);

        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        grass_shader.setMat4("projection", projection);

        //glm::mat4 view = camera.GetViewMatrix();
        grass_shader.setMat4("view", view);

        grass_shader.setFloat("_Time", currentFrame);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grass_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, grass_mask_texture);

        glBindBuffer(GL_ARRAY_BUFFER, ground_VBO);
        glDrawArrays(GL_POINTS, 0, ground_vertex_num);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &ground_VAO);
    glDeleteBuffers(1, &ground_VBO);
    glDeleteBuffers(1, &ground_EBO);

    glfwTerminate();

    delete[] ground_vertex_data;
    delete[] ground_index;
}

