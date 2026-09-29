// Trakaliuk Anzhelika K-28
// MSVC v143 (версія 19.44.35228, 32-bit x86)
// Variant 3

#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <algorithm>
#include <execution>
#include <iomanip>
#include <limits>

struct Timer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::string exp_name;
	double* out_time;
public:

	Timer(const std::string& name, double* out_time = nullptr) : exp_name(name),
		start(std::chrono::high_resolution_clock::now()),
		out_time(out_time)
	{ ; }
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


// Generates random integer number sequence and stores it in provided file.
// Writes quantity of generated numbers as the first number in file.
void gen_random_num(const std::string& filename,size_t quantity, int min_val, int max_val) {
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


// Custom parallel none_of algorithm
template <typename Predicate>
// The function takes vector of generated numbers, a number of threads and a predicate
bool custom_none_of(const std::vector<int>& exp_data, size_t K, Predicate pred) {
	size_t chunk_count = exp_data.size() / K;

	std::vector<int> thread_results(K, 0);
	std::vector<std::thread> threads;

	for (size_t i = 0; i < K; i++) {
		std::vector<int>::const_iterator tbegin = exp_data.begin() + i * chunk_count;
		std::vector<int>::const_iterator tend = (i == K - 1) ? exp_data.end() : tbegin + chunk_count;

		threads.emplace_back([pred, tbegin, tend, i, &thread_results]() {
			bool result = std::none_of(tbegin, tend, pred);
			thread_results[i] = result ? 1 : 0;
			});

	}
	for (auto& t : threads) {
		t.join();
	}

	return std::all_of(thread_results.begin(), thread_results.end(), [](int res) {
		return res;
		});
}


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
	
	auto predicate = [](int num) {
		return (num > 20000); // entered this value, because we want to iterate through full vector
		};

	// Testing library algorithm excecution speed
	std::cout << "Hardware threads: " << std::thread::hardware_concurrency() << "\n";
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
		"|" << "Time(ms) \n";

	size_t k = 16;
	for (size_t i = 1; i <= k; i++) {
		double curr_time;
		double curr_k = i;
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
	std::cout << "Time: " << best_time << "\n";



}




