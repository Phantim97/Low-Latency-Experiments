#include <fstream>
#include <memory>
#include <array>
#include <random>
#include <omp.h>

#include <benchmark/benchmark.h>

#include "ScopedTimer.h"

//Prompt:
/**
 * \brief
 *  Write a program that does the following:
 *	Read a double precision number (from stdin) and print the smallest number that is larger than 95% of numbers encountered so far.
 *	Run program on input size = 128,000,000 and provide the time taken. Provide the time taken for this random input along with the solution.
 * \return 
 */

static void generate_sample_data(const int sz)
{
	ScopedTimer<std::chrono::milliseconds> st;
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
		//	ScopedTimer<std::chrono::milliseconds> st;
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

//TODO:
/*
-Structure to capture x percentile range
-calculate on the fly
-Implement a skip list
*/

//BENCHMARK(BM_generate_sample_data)->Arg(128000000);
//BENCHMARK(BM_generate_sample_data_parallel)->Arg(128000000);

//BENCHMARK_MAIN();
int main()
{
	generate_sample_data(128000000);

	return 0;
}