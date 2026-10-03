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
	const std::string filename1 = "data1.txt";
	const std::string filename2 = "data2.txt";
	const std::string filename3 = "data3.txt";

	std::vector<int> generated_nums1;
	std::vector<int> generated_nums2;
	std::vector<int> generated_nums3;

	try {
		std::cout << "Generating files...\n";
		gen_random_num(filename1, 100000, 1, 10000);
		get_num_from_file(generated_nums1, filename1);
		gen_random_num(filename2, 1000000, 1, 10000);
		get_num_from_file(generated_nums2, filename2);
		gen_random_num(filename3, 10000000, 1, 10000);
		get_num_from_file(generated_nums3, filename3);
		std::cout << "Generation complete.\n";
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR" << e.what() << "\n";
		return 1;
	}

	bool(*pred_worst)(int) = [](int num) {
		return num > 20000;
		};

	bool(*pred_early)(int) = [](int num) {
		return num == 5000;
		};

	size_t k = 30;
	std::cout << "------TEST1(worst-case)------\n";
	run_test(generated_nums1, pred_worst, k);
	std::cout << "------TEST1(early exit)------\n";
	run_test(generated_nums1, pred_early, k);
	std::cout << "------TEST2(worst-case)------\n";
	run_test(generated_nums2, pred_worst, k);
	std::cout << "------TEST2(early exit)------\n";
	run_test(generated_nums2, pred_early, k);
	std::cout << "------TEST3(worst-case)------\n";
	run_test(generated_nums3, pred_worst, k);
	std::cout << "------TEST3(early exit)------\n";
	run_test(generated_nums3, pred_early, k);

}



