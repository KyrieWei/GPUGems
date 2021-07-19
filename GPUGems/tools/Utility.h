#pragma once
#include <vector>
#include <string>

double random_num(double min, double max);

std::vector<std::string> split(const std::string& line, char c);

void generate_mesh(const char* str, int res_x, int res_y, double width, double height);
void generate_mesh_with_heightmap(const char* str, int res_x, int res_y, double ground_x, double ground_y, double ground_z, const unsigned char* height_map, int height_map_width, int height_map_height, int height_map_channel);

void load_mesh(const char* str, float* vertex, unsigned int* index);

