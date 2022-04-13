#include <fstream>
#include <memory>
#include <vector>
#include <random>
#include <omp.h>

#include <benchmark/benchmark.h>

#include "ScopedTimer.h"
#include "numeric_types.h"

//Prompt:
/**
 * \brief
 *  Write a program that does the following:
 *	Read a double precision number (from stdin) and print the smallest number that is larger than 95% of numbers encountered so far.
 *	Run program on input size = 128,000,000 and provide the time taken. Provide the time taken for this random input along with the solution.
 * \return 
 */

//Note what they maybe mean by stdin is to calculate it per input as we grow the list

//TODO: Implement indexed version of skip list

template <typename T, typename = numeric_t<T>>
struct Node
{
	T key;
	Node<T>** forward; //Array to hold pointers

	Node(T key, const int level)
	{
		this->key = key;
		forward = new Node<T>*[level + 1];

		memset(forward, 0, sizeof(Node<T>*) * (level + 1));
	}

	~Node()
	{
		delete[] forward;
	}
};

static enum class Scale
{
	larger_than,
	smaller_than,
};

template<typename T, typename = numeric_t<T>>
class SkipList
{
private:
	int max_lvl_;
	float p_; //fraction of nodes with level i pointers having level i+1 pointers
	Node<T>* header_ = nullptr;
	int level_;
	int sz_ = 0;

	int random_level() const
	{
		//rand engine
		std::random_device rd;
		const std::uniform_real_distribution<T> unif(0, RAND_MAX);
		std::default_random_engine re(rd());
		double r = unif(re) / RAND_MAX;
		int lvl = 0;

		while (r < p_ && lvl < max_lvl_)
		{
			lvl++;
			r = unif(re) / RAND_MAX;
		}

		return lvl;
	}

public:
	SkipList(int max_lvl, float p)
	{
		this->max_lvl_ = max_lvl;
		this->p_ = p;
		this->level_ = 0;
		this->header_ = new Node<T>(-1, max_lvl);
	}

	~SkipList()
	{
		delete header_;
	}

	Node<T>* create_node(T key, int level)
	{
		Node<T>* n = new Node<T>(key, level);
		return n;
	}

	void insert(T key)
	{
		Node<T>* current = header_;

		//Create update array and initialize it
		Node<T>* update[max_lvl_ + 1];
		memset(update, 0, sizeof(Node<T>*) * (max_lvl_ + 1));

		for (int i = level_; i >= 0; i--)
		{
			while (current->forward[i] != nullptr && current->forward[i]->key < key)
			{
				current = current->forward[i];
			}

			update[i] = current;
		}

		//Reached level 0 and forward pointer to right which is desired position to insert key
		current = current->forward[0];

		if (current == nullptr || current->key != key)
		{
			const int rlevel = random_level();

			if (rlevel > level_)
			{
				for (int i = level_ + 1; i < rlevel + 1; i++)
				{
					update[i] = header_;
				}

				level_ = rlevel;
			}

			Node<T>* n = create_node(key, rlevel);
			sz_++;

			//Insert node by rearranging pointers
			for (int i = 0; i <= rlevel; i++)
			{
				n->forward[i] = update[i]->forward[i];
				update[i]->forward[i] = n;
			}
		}
	}

	void remove(T key)
	{
		Node<T>* current = header_;

		Node<T>* update[max_lvl_ + 1];
		memset(update, 0, sizeof(Node<T>*) * (max_lvl_ + 1));

		for (int i = level_; i >= 0; i--)
		{
			while(current->forward[i] != nullptr && current->forward[i]->key < key)
			{
				current = current->forward[i];
			}

			update[i] = current;
		}

		current = current->forward[0];

		if (current != nullptr && current->key == key)
		{
			for (int i = 0; i <= level_; i++)
			{
				if (update[i]->forward[i] != current)
				{
					break;
				}

				update[i]->forward[i] = current->forward[i];
			}

			//Remove levels having no elements
			while (level_ > 0 && header_->forward[level_] == 0)
			{
				level_--;
			}
		}
	}

	void display_list()
	{
		for (int i = 0; i <= level_; i++)
		{
			Node<T>* node = header_->forward[i];
			std::cout << "Level: " << i << '\n';

			while (node != nullptr)
			{
				std::cout << node->key << " ";
				node = node->forward[i];
			}

			std::cout << '\n';
		}
	}

	T percentile_find(Scale s, double percent)
	{
		percent = sz_ * percent;

		if (percent < 0 || percent > sz)
		{
			return DBL_MIN;
		}

		Node<T>* current = header_;
		int pos = 0;
		int tmp = 0;


		return curr->value;
	}

	void find(T key)
	{
		Node<T>* current = header_;

		for (int i = level_; i >= 0; i--)
		{
			while (current->forward[i] && current->forward[i]->key < key)
			{
				current = current->forward[i];
			}
		}

		current = current->forward[0];

		if (current && current->key == key)
		{
			std::cout << "Found: " << key << '\n';
		}
	}

	size_t size() const
	{
		return sz_;
	}
};

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

//TODO:
/*
-Structure to capture x percentile range
-calculate on the fly
-Implement a skip list
*/

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <limits>

//SFINAE out all non numeric types for this class
template <typename T, typename = numeric_t<T>>
class MinMaxHeap
{
private:
	
	std::priority_queue<T, std::vector<T>, std::greater<>> min_heap_; //All elements above percentile go here
	std::priority_queue<T> max_heap_; //All elements below percentile go here
	double percentile_ = 0;
public:
	MinMaxHeap(const double percentile) : percentile_(percentile)
	{
	}

	void insert(T val)
	{
		min_heap_.push(val);
		const double total = min_heap_.size() + max_heap_.size();
		const double ratio = max_heap_.size() / total;

		//If ratio is less than percentile we move it out of the min heap
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

	std::size_t size() const
	{
		return min_heap_.size() + max_heap_.size();
	}
};

int main(int argc, char* argv[])
{
	//const double d = std::stod(argv[1]);
	constexpr double d = 95;
	MinMaxHeap<double> p(d / 100);
	double n;

	//size_t i = 0;

	//while (std::cin >> n)
	//{
	//	i++;
	//	p.insert(n);
	//	std::cout << p.get() << '\n';
	//}

	for (int i = 0; i < 10000; i++)
	{
		p.insert(i);
	}

	std::cout << "Total Size: " << '\n';

	std::cout << p.get() << '\n';
	std::cout << p.size() << '\n';

	return 0;
}

//int main()
//{
//	const std::vector<double> data = generate_sample_data<double>(30);
//	SkipList<double> sl(3, 0.5);
//
//	for (int i = 0; i < data.size(); i++)
//	{
//		sl.insert(data[i]);
//	}
//
//	sl.display_list();
//	std::cout << "Size: " << sl.size() << '\n';
//
//	std::cout << "Result: " << sl.percentile_find(Scale::larger_than, .95) << '\n';*/
//
//	return 0;
//}