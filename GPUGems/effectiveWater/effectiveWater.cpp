#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#include "effectiveWater.h"

const int num_func = 3;

template<typename T>
T RandT(T _min, T _max)
{
    T temp;
    if (_min > _max)
    {
        temp = _max;
        _max = _min;
        _min = temp;
    }

    return rand() / (double)RAND_MAX * (_max - _min) + _min;
}

std::vector<float> random_dir()
{
    float u = RandT<float>(0, RAND_MAX);
    float cos = std::cos(u);
    float sin = std::sin(u);
    std::vector<float> vec = { cos, sin };
    return vec;
}

std::vector<float> random_dir(float theta_min, float theta_max)
{
    float u = RandT<float>(theta_min, theta_max);
    float cos = std::cos(u);
    float sin = std::sin(u);
    std::vector<float> vec = { cos, sin };
    return vec;
}

std::vector<std::string> split(const std::string& line, char c)
{
    std::vector<std::string> vec;
    int start = 0, end = 0;
    while (end != line.length())
    {
        if (line[end] != c)
            end++;
        else
        {
            vec.push_back(line.substr(start, end - start));

            start = end + 1;
            end++;
        }
    }
    if (start < end)
        vec.push_back(line.substr(start, end - start));

    return vec;
}

void generate_mesh(const char* str, int width, int height)
{
    std::ofstream out(str);

    double length_x = 2.0, length_z = 2.0;

    double delta_x = length_x / width;
    double delta_z = length_z / height;

    double x, y, z;
    double start_x = -length_x / 2.0, start_z = -length_z / 2.0;

    out << "#vertex_num: " << (width + 1) * (height + 1) << std::endl;
    out << "#triangle_num: " << width * height * 2 << std::endl;
    out << std::endl;

    for (int i = 0; i < width + 1; i++)
    {
        for (int j = 0; j < height + 1; j++)
        {
            x = start_x + delta_x * i;
            y = 0;
            z = start_z + delta_z * j;

            out << "v " << x << " " << y << " " << z << std::endl;
        }
    }
    
    out << std::endl;

    int ind_0, ind_1, ind_2;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            ind_0 = i * (height + 1) + j + 1;
            ind_1 = i * (height + 1) + j + 2;
            ind_2 = (i + 1) * (height + 1) + j + 1;

            out << "f " << ind_0 << " " << ind_1 << " " << ind_2 << std::endl;

            ind_0 = i * (height + 1) + j + 2;
            ind_1 = (i + 1) * (height + 1) + j + 2;
            ind_2 = (i + 1) * (height + 1) + j + 1;

            out << "f " << ind_0 << " " << ind_1 << " " << ind_2 << std::endl;
        }
    }

    out.close();
}

void load_mesh(const char* str, float* vertex, unsigned int* index)
{
    std::ifstream m_file(str);
    std::string line;
    std::vector<std::string> split_line;
    if (m_file.is_open())
    {
        int i = 0, j = 0;
        while (std::getline(m_file, line))
        {
            if (line.empty())
                continue;
            if (line[0] == '#')
                continue;

            split_line = split(line, ' ');
            
            if (split_line[0] == "v")
            {
                vertex[i++] = std::stof(split_line[1]);
                vertex[i++] = std::stof(split_line[2]);
                vertex[i++] = std::stof(split_line[3]);
            }

            else if (split_line[0] == "f")
            {
                index[j++] = static_cast<unsigned int>(std::stoi(split_line[1]) - 1);
                index[j++] = static_cast<unsigned int>(std::stoi(split_line[2]) - 1);
                index[j++] = static_cast<unsigned int>(std::stoi(split_line[3]) - 1);
            }
            else
            {

            }
        }
        m_file.close();
    }
    else
    {
        std::cout << "failed to open file " << str << std::endl;
    }
}

void Gerstner(float* vertex, int width, int height, float time, const std::vector<float>& amplitude, const std::vector<float>& wavelength, const std::vector<float>& speed, const std::vector<std::vector<float>>& dir)
{
    int len = amplitude.size();
    float omega, y_pos;
 
    for (int i = 0; i < width + 1; i++)
    {
        for (int j = 0; j < height + 1; j++)
        {
            y_pos = 0;
            for (int n = 0; n < len; n++)
            {
                omega = 2.0f / wavelength[n];
                y_pos += amplitude[n] * sin(omega * (dir[n][0] * i + dir[n][1] * j) + time * speed[n] * omega);
            }
            
            vertex[(i * (height + 1) + j) * 3 + 1] = y_pos;
        }
    }
}

void Gerstner(float* vertex, int width, int height, float time)
{
    float amplitude = 0.05f;
    float wavelength = 4.0f;
    float omega = 2.0 / wavelength;
    float speed = 4.0f;
    float y_pos;

    float dir[2] = { 0.5f, 0.5f };

    for (int i = 0; i < width + 1; i++)
    {
        for (int j = 0; j < height + 1; j++)
        {
            y_pos = amplitude * sin(omega * (dir[0] * i + dir[1] * j) + time * speed * omega);
            vertex[(i * (height + 1) + j) * 3 + 1] = y_pos;
        }
    }
}

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


    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Gerstner(water_mesh_vertex, res_x, res_y, currentFrame, amplitude, wavelength, speed, dir);
        //Gerstner(water_mesh_vertex, res_x, res_y, currentFrame);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_num * 3, water_mesh_vertex, GL_DYNAMIC_DRAW);

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


