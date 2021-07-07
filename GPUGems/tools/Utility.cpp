#include <iostream>
#include <fstream>

#include "Utility.h"

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

void generate_mesh(const char* str, int res_x, int res_y, double width, double height)
{
    std::ofstream out(str);

    double length_x = width, length_z = height;

    double delta_x = length_x / res_x;
    double delta_z = length_z / res_y;

    double x, y, z;
    double start_x = -length_x / 2.0, start_z = -length_z / 2.0;

    out << "#vertex_num: " << (res_x + 1) * (res_y + 1) << std::endl;
    out << "#triangle_num: " << res_x * res_y * 2 << std::endl;
    out << std::endl;

    for (int i = 0; i < res_x + 1; i++)
    {
        for (int j = 0; j < res_y + 1; j++)
        {
            x = start_x + delta_x * i;
            y = 0;
            z = start_z + delta_z * j;

            out << "v " << x << " " << y << " " << z << std::endl;
        }
    }

    out << std::endl;

    int ind_0, ind_1, ind_2;

    for (int i = 0; i < res_x; i++)
    {
        for (int j = 0; j < res_y; j++)
        {
            ind_0 = i * (res_y + 1) + j + 1;
            ind_1 = i * (res_y + 1) + j + 2;
            ind_2 = (i + 1) * (res_y + 1) + j + 1;

            out << "f " << ind_0 << " " << ind_1 << " " << ind_2 << std::endl;

            ind_0 = i * (res_y + 1) + j + 2;
            ind_1 = (i + 1) * (res_y + 1) + j + 2;
            ind_2 = (i + 1) * (res_y + 1) + j + 1;

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