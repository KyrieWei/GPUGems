#include "waterCaustic.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../tools/stb_image_write.h"

#include "../tools/Utility.h"
#include "../tools/Maths.h"

void generate_png(int alpha)
{
    int sky_texture_width, sky_texture_height, sky_texture_channel;
    unsigned char* sky_texture_data = stbi_load("waterCaustic/assets/sky.tga", &sky_texture_width, &sky_texture_height, &sky_texture_channel, 0);
    unsigned char* sky_image = new unsigned char[sky_texture_width * sky_texture_height * 4];

    int sky_index = 0, sky_ori_index = 0;
    for (int j = sky_texture_height - 1; j >= 0; j--)
    {
        for (int i = 0; i < sky_texture_width; i++)
        {
            sky_image[sky_index++] = sky_texture_data[sky_ori_index++];
            sky_image[sky_index++] = sky_texture_data[sky_ori_index++];
            sky_image[sky_index++] = sky_texture_data[sky_ori_index++];
            sky_image[sky_index++] = (unsigned char)alpha;
        }
    }

    stbi_write_png("waterCaustic/assets/sky_transparent.png", sky_texture_width, sky_texture_height, 4, sky_image, sky_texture_width * 4);

    stbi_image_free(sky_image);
    stbi_image_free(sky_texture_data);
}

void show_water_caustic(GLFWwindow* window, Camera& camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, processInputCallback processInput)
{
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

#pragma region water_ground

    Shader ground_shader("waterCaustic/shaders/ground_vert.vs", "waterCaustic/shaders/ground_frag.fs");

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
    unsigned int ground_texture;
    glGenTextures(1, &ground_texture);
    glBindTexture(GL_TEXTURE_2D, ground_texture);

    //set wrap and filter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int ground_width, ground_height, ground_nrChannels;
    unsigned char* data = stbi_load("waterCaustic/assets/MURO_2.tga", &ground_width, &ground_height, &ground_nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ground_width, ground_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texutre" << std::endl;
    }
    stbi_image_free(data);

#pragma endregion

#pragma region water_surface

    

    Shader water_surface_shader("waterCaustic/shaders/water_surface_vert.vs", "waterCaustic/shaders/water_surface_frag.fs");

    int res_x = 200, res_y = 200;
    int vertex_num = (res_x + 1) * (res_y + 1);
    int tri_num = res_x * res_y * 2;
    int stride = 5;

    float* water_surface_vertex = new float[vertex_num * stride];
    unsigned int* water_surface_index = new unsigned int[tri_num * 3];

    //generate_mesh("waterCaustic/assets/water_surface.txt", res_x, res_y, 4, 4);

    load_mesh("waterCaustic/assets/water_surface.txt", water_surface_vertex, water_surface_index);

    unsigned int surface_VAO, surface_VBO, surface_EBO;
    glGenVertexArrays(1, &surface_VAO);
    glGenBuffers(1, &surface_VBO);
    glGenBuffers(1, &surface_EBO);

    glBindVertexArray(surface_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, surface_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_num * stride, water_surface_vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tri_num * 3, water_surface_index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int sky_texture;
    glGenTextures(1, &sky_texture);

    glBindTexture(GL_TEXTURE_2D, sky_texture);

    //set wrap and fliter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //generate_png(140);

    int sky_texture_width, sky_texture_height, sky_texture_channel;
    unsigned char* sky_texture_data = stbi_load("waterCaustic/assets/sky_transparent.png", &sky_texture_width, &sky_texture_height, &sky_texture_channel, 0);


    if (sky_texture_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sky_texture_width, sky_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sky_texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load sky texture" << std::endl;
    }
    stbi_image_free(sky_texture_data);

    //wave.xy = direction
    //wave.z = amplitude
    //wave.w = wavelength

    const int num_func = 1;

    glm::vec4 wave[num_func];
    float speed[num_func];

    for (int i = 0; i < num_func; i++)
    {
        std::vector<float> dir = random_dir(-30, -60);
        wave[i].x = dir[0];
        wave[i].y = dir[1];
        wave[i].z = RandT<float>(0.05, 0.07);
        wave[i].w = RandT<float>(1.0, 1.5);

        speed[i] = RandT<float>(0.1, 0.3);
    }

    float Q = 0.4;

    unsigned int water_surface_ubi = glGetUniformBlockIndex(water_surface_shader.ID, "params");
    glUniformBlockBinding(water_surface_shader.ID, water_surface_ubi, 0);

    unsigned int water_surface_uboBlock;
    glGenBuffers(1, &water_surface_uboBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, water_surface_uboBlock);
    glBufferData(GL_UNIFORM_BUFFER, num_func * sizeof(glm::vec4) + num_func * sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, water_surface_uboBlock, 0, num_func * sizeof(glm::vec4) + num_func * sizeof(float));

    glBindBuffer(GL_UNIFORM_BUFFER, water_surface_uboBlock);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, num_func * sizeof(glm::vec4), wave);
    glBufferSubData(GL_UNIFORM_BUFFER, num_func * sizeof(glm::vec4), num_func * sizeof(float), speed);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    water_surface_shader.use();
    water_surface_shader.setFloat("k", 2.5);
    water_surface_shader.setFloat("Q", Q);

#pragma endregion

    Shader water_caustic_shader("waterCaustic/shaders/water_caustic_vert.vs", "waterCaustic/shaders/water_caustic_frag.fs");

    unsigned int sun_texture;
    glGenTextures(1, &sun_texture);

    glBindTexture(GL_TEXTURE_2D, sun_texture);

    //set wrap and fliter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int sun_texture_width, sun_texture_height, sun_texture_channel;
    unsigned char* sun_texture_data = stbi_load("waterCaustic/assets/sky3.tga", &sun_texture_width, &sun_texture_height, &sun_texture_channel, 0);

    if (sun_texture_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sun_texture_width, sun_texture_height, 0, GL_RED, GL_UNSIGNED_BYTE, sun_texture_data);
    }
    else
    {
        std::cout << "failed to load sun texture" << std::endl;
    }

    stbi_image_free(sun_texture_data);

    glm::vec3 lightColor = glm::vec3(0.7, 0.7, 0.7);

    water_caustic_shader.use();
    water_caustic_shader.setFloat("k", 2.5);
    water_caustic_shader.setFloat("Q", Q);
    water_caustic_shader.setVec3("lightColor", lightColor);

    //MSAA
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    camera.setInitialStatus(glm::vec3(0, 2.7, 5.5), glm::vec3(-0.02, 0.9, 0.4), -93, -23);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //water ground
        ground_shader.use();
        glm::mat4 model = glm::mat4(1.0);
        ground_shader.setMat4("model", model);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ground_shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ground_shader.setMat4("view", view);

        glBindTexture(GL_TEXTURE_2D, ground_texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, water_ground_index_num, GL_UNSIGNED_INT, 0);

        //water caustic
        water_caustic_shader.use();
        model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.001, 0.0));

        water_caustic_shader.setMat4("model", model);
        water_caustic_shader.setMat4("projection", projection); 
        water_caustic_shader.setMat4("view", view);

        water_caustic_shader.setFloat("_Time", currentFrame);

        glBindTexture(GL_TEXTURE_2D, sun_texture);

        glBindVertexArray(surface_VAO);
        glDrawElements(GL_TRIANGLES, tri_num * 3, GL_UNSIGNED_INT, 0);

        //water surface
        water_surface_shader.use();

        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        water_surface_shader.setMat4("model", model);
        water_surface_shader.setMat4("projection", projection);
        water_surface_shader.setMat4("view", view);

        water_surface_shader.setFloat("_Time", currentFrame);

        glBindTexture(GL_TEXTURE_2D, sky_texture);

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