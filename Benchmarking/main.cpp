#include <fstream>
#include <memory>
#include <array>
#include <random>
#include <omp.h>

#include <benchmark/benchmark.h>

static void BM_generate_sample_data(benchmark::State& state)
{
	//	ScopedTimer<std::chrono::milliseconds> st;
	for (benchmark::State::StateIterator::Value _ : state)
	{
		const int sz = state.range(0);
		std::vector<double> a(sz);

		constexpr double lower_bound = 0;
		constexpr double upper_bound = 10000;
		const std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
		std::default_random_engine re;

		//#pragma omp parallel for (shared:a)
		for (int i = 0; i < a.size(); i++)
		{
			a[i] = unif(re);
		}
	}
}

static void BM_generate_sample_data_parallel(benchmark::State& state)
{
	for (benchmark::State::StateIterator::Value _ : state)
	{
		omp_set_num_threads(8);
		const int sz = state.range(0);
		std::vector<double> a(sz);

		constexpr double lower_bound = 0;
		constexpr double upper_bound = 10000;
		const std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
		std::default_random_engine re;

		#pragma omp parallel for (shared:a)
		for (int i = 0; i < a.size(); i++)
		{
			a[i] = unif(re);
		}
	}
}

BENCHMARK(BM_generate_sample_data)->Arg(128000000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_generate_sample_data_parallel)->Arg(128000000)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();