// Trakaliuk Anzhelika K-28
// MSVC v143 (версія 19.44.35228, 32-bit x86)
// Variant 3

#include <iostream>
#include <string>
#include "data_gen.h"
#include "custom_none_of.h"
#include "test.h"


int main() {
	// Generating number and storing them in the vector
	const std::string filename = "data.txt";
	std::vector<int> generated_nums;
	try {
		gen_random_num(filename, 10000000, 1, 10000);
		get_num_from_file(generated_nums, filename);
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR" << e.what() << "\n";
		return 1;
	}

	bool(*predicate)(int) = [](int num) {
		return (num > 20000); // entered this value, because we want to iterate through full vector
		};

}



