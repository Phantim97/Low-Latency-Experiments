#include <fstream>
#include <vector>
#include <random>
#include <omp.h>

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <limits>
#include <string>

#include "ScopedTimer.h"
#include "numeric_types.h"

template<typename T, typename = numeric_t<T>>
static std::vector<T> generate_sample_data(const int sz)
{
	ScopedTimer<std::chrono::milliseconds> st;
	std::vector<T> a(sz);

	constexpr double lower_bound = 0;
	constexpr double upper_bound = 10000;
	std::random_device rd;
	const std::uniform_real_distribution<T> unif(lower_bound, upper_bound);
	std::default_random_engine re(rd());

	#pragma omp parallel for (shared:a)
	for (int i = 0; i < a.size(); i++)
	{
		a[i] = unif(re);
	}

	return a;
}

//SFINAE out all non numeric types for this class
template <typename T, typename = numeric_t<T>>
class MinMaxHeap
{
private:
	
	std::priority_queue<T, std::vector<T>, std::greater<>> min_heap_; //All elements above percentile go here
	std::priority_queue<T> max_heap_; //All elements below percentile go here
	double percentile_ = 0;
public:
	explicit MinMaxHeap(const double percentile) : percentile_(percentile)
	{
	}

	void insert(T val)
	{
		min_heap_.push(val);
		const double total = min_heap_.size() + max_heap_.size();
		const double ratio = max_heap_.size() / total;

		//If ratio is less than percentile we move value out of the min heap
		if (ratio < percentile_)
		{
			const double downgraded = min_heap_.top();
			min_heap_.pop();
			max_heap_.push(downgraded);
		}

		if (min_heap_.empty() || max_heap_.empty())
		{
			return;
		}

		T min_top = min_heap_.top();
		T max_top = max_heap_.top();

		if (min_top < max_top)
		{
			min_heap_.pop();
			max_heap_.pop();
			min_heap_.push(max_top);
			max_heap_.push(min_top);
		}
	}

	T get()
	{
		if (!min_heap_.empty())
		{
			return min_heap_.top();
		}

		std::cout << "Min heap is empty\n";
		return std::numeric_limits<T>::quiet_NaN();
	}

	size_t size() const
	{
		return min_heap_.size() + max_heap_.size();
	}

	double min_heap_ratio() const
	{
		return 1.0 * min_heap_.size() / this->size();
	}
};

template <typename T>
static void add_and_find(MinMaxHeap<T>& heap, std::vector<T>& data)
{
	ScopedTimer<std::chrono::milliseconds> st;

	for (int i = 0; i < data.size(); i++)
	{
		heap.insert(data[i]);
	}

	std::cout << heap.get() << '\n';
}

template <typename T>
static void heap_populate_and_find(MinMaxHeap<T>& h)
{
	ScopedTimer<std::chrono::milliseconds> st;
	size_t i = 0;
	double n;

	while (std::cin >> n)
	{
		i++;
		h.insert(n);

		if (i % 2000000 == 0)
		{
			std::cout << "Smallest number > 95% of set [" << i <<"]: " << h.get() << '\n';
		}
	}

	std::cout << "Smallest number > 95% of set [final]: " << h.get() << '\n';
}

int main(const int argc, char* argv[])
{
	if (argc == 2) //stdin version
	{
		const double d = std::stod(argv[1]);
		MinMaxHeap<double> heap(d / 100);
		std::cout << "Inserting into list...\n";
		heap_populate_and_find(heap);
		std::cout << "Min-Heap ratio to total size: " << heap.min_heap_ratio() << '\n'; //Shows percentage stored in min heap
	}
	else //Vector of uniform random values
	{
		MinMaxHeap<double> heap(95 / 100.0);
		std::cout << "Generating dataset...\n";
		std::vector<double> v = generate_sample_data<double>(128000000);
		std::cout << "Finding smallest value larger than 95% of set...\n";
		add_and_find(heap, v);
		std::cout << "Total Size: " << heap.size() <<'\n';
		std::cout << "Smallest number > 95% of set: " << heap.get() << '\n';
		std::cout << "Min-Heap ratio to total size: " << heap.min_heap_ratio() << '\n'; //Shows percentage stored in min heap
	}

	return 0;
}