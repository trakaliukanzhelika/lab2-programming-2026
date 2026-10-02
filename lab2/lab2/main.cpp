// Trakaliuk Anzhelika K-28
// MSVC v143 (версія 19.44.35228, 32-bit x86)
// Variant 3

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <execution>
#include <iomanip>
#include <limits>
#include "data_gen.h"
#include "custom_none_of.h"

struct Timer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::string exp_name;
	double* out_time;
public:

	Timer(const std::string& name, double* out_time = nullptr) : exp_name(name),
		start(std::chrono::high_resolution_clock::now()),
		out_time(out_time)
	{
		;
	}
	// If the pointer was provided, assigns the duration to the specific variable.
	// Otherwise, outputs the experiment`s name and duration.
	~Timer() {
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::high_resolution_clock::now() - start).count();
		double ms = duration * 0.001;
		if (out_time) {
			*out_time = ms;
		}
		else {
			std::cout << exp_name << ": " << ms << "ms" << "\n";
		}
	}
};


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

	// Testing library algorithm excecution speed
	size_t hw = std::thread::hardware_concurrency();
	std::cout << "Hardware threads: " << hw << "\n";
	std::cout << "---LIBRARY ALGORITHM std::none_of() ---\n";
	{
		Timer t("No policy: ");
		volatile bool res = std::none_of(generated_nums.begin(), generated_nums.end(), predicate);
	}
	{
		Timer t("Policy seq: ");
		volatile bool res = std::none_of(std::execution::seq, generated_nums.begin(), generated_nums.end(), predicate);
	}
	{
		Timer t("Policy par: ");
		volatile bool res = std::none_of(std::execution::par, generated_nums.begin(), generated_nums.end(), predicate);
	}
	{
		Timer t("Policy unseq: ");
		volatile bool res = std::none_of(std::execution::par_unseq, generated_nums.begin(), generated_nums.end(), predicate);
	}


	// Testing custom algorithm excecution speed on different amount of threads
	std::cout << "---CUSTOM PARALLEL ALGORITHM---\n";
	double best_time = std::numeric_limits <double> ::infinity();
	size_t best_k;

	std::cout << std::left << std::setw(5) << "K" <<
		"|" << " Time(ms) \n";

	size_t k = 16;
	for (size_t i = 1; i <= k; i++) {
		double curr_time;
		size_t curr_k = i;
		{
			Timer t("", &curr_time);
			volatile bool res = custom_none_of(generated_nums, i, predicate);
		}
		std::cout << std::left << std::setw(5) << curr_k << "| " << curr_time << "\n";

		if (curr_time < best_time) {
			best_time = curr_time;
			best_k = curr_k;
		}
	}

	std::cout << "---BEST VALUES---\n";
	std::cout << "Number of threads: " << best_k << "\n";
	std::cout << "Ratio (Best K / Hardware threads): " << static_cast<double>(best_k) / hw
		<< " (" << best_k << " / " << hw << ")\n";
	std::cout << "Time: " << best_time << "\n";

}



