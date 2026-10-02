#pragma once

#include <string>
#include <vector>

void gen_random_num(const std::string& filename, size_t quantity, int min_val, int max_val);
void get_num_from_file(std::vector<int>& exp_data, const std::string& filename);