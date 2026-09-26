// Trakaliuk Anzhelika K-28
// MSVC v143 (версія 19.44.35228, 32-bit x86)
// Variant 3

#include <chrono>
#include <iostream>
#include <string>

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
