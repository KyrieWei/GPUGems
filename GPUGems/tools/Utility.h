#pragma once
#include <vector>
#include <string>

std::vector<std::string> split(const std::string& line, char c);

void generate_mesh(const char* str, int res_x, int res_y, double width, double height);

void load_mesh(const char* str, float* vertex, unsigned int* index);

