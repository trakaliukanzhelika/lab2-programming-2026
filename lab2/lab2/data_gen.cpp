#include "data_gen.h"
#include <iostream>
#include <random>
#include <fstream>
#include <stdexcept>


// Generates random integer number sequence and stores it in provided file.
// Writes quantity of generated numbers as the first number in file.
void gen_random_num(const std::string& filename, size_t quantity, int min_val, int max_val) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min_val, max_val);

	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("Unable to open the file for writing!");
	}
	out << quantity << "\n";
	for (size_t i = 0; i < quantity; i++) {
		out << dist(gen) << "\n";
	}
}


// Reads generated numbers from provided file and stores them in provided vector.
// Resizes vector to needed capacity.
void get_num_from_file(std::vector<int>& exp_data, const std::string& filename) {
	std::ifstream f(filename);
	if (!f.is_open()) {
		throw std::runtime_error("Unable to open the file with generated numbers!");
	}
	size_t count;
	f >> count;
	exp_data.resize(count);
	for (size_t i = 0; i < count; i++) {
		f >> exp_data[i];
	}
	if (f.fail()) {
		throw std::runtime_error("Something wrong with the reading file!");
	}
}
