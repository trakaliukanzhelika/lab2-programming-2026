#pragma once

#include <vector>

void run_test(std::vector<int>& generated_nums, bool(*predicate)(int), size_t k);