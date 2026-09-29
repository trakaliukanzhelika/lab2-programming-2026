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
		return res == 1;
		});
}







