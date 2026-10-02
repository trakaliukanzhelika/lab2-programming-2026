#include "custom_none_of.h"
#include <thread>
#include <algorithm>

// Custom parallel none_of algorithm
// The function takes vector of generated numbers, a number of threads and a predicate
bool custom_none_of(const std::vector<int>& exp_data, size_t K, bool(*pred)(int)) {
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