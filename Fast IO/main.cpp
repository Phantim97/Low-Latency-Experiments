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

//Note what they maybe mean by stdin is to calculate it per input as we grow the list

template <typename T>
struct Node
{
	T key;

	//Array to hold pointers
	Node<T>** forward;
	Node(int key, int level)
	{
		this->key = key;
		forward = new Node<T>*[level + 1];

		memset(forward, 0, sizeof(Node<T>*) * (level + 1));
	}
};

//O(sqrt N)
template<typename T>
class SkipList
{
private:
	int max_lvl_;
	float p_; //fraction of nodes with level i pointers having level i+1 pointers
	Node<T>* header_ = nullptr;
	int level_;

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
		//TODO: iteratively delete whole skip list
	};

	int random_level()
	{
		//rand engine
		const std::uniform_real_distribution<double> unif(0, max_lvl_);
		std::default_random_engine re;
		double r = unif(re);
		int lvl = 0;

		while (r < p_ && lvl < max_lvl_)
		{
			lvl++;
			r = unif(re);
		}

		return lvl;

	}

	Node<T>* create_node(T key, int level)
	{
		Node<T>* n = new Node<T>(key, level);
		return n;
	}

	void insert_element(T key)
	{
		Node<T>* current = header_;

		//Create update array and initialize it
		Node<T>* update[max_lvl_ + 1];
		memset(update, 0, sizeof(Node<T>*) * (max_lvl_ + 1));

		for (int i = level; i >= 0; i--)
		{
			while (current->forward[i] != NULL && current->forward[i]->key < key)
			{
				current = current->forward[i];
				update[i] = current;
			}
		}

		//Reached level 0 and forward pointer to right which is desired position to insert key
		current = current->forward[0];

		if (current == NULL || current->key != key)
		{
			int rlevel = random_level();

			if (rlevel > level_)
			{
				for (int i = level_ + 1; i < rlevel + 1; i++)
				{
					update[i] = header_;
				}

				level_ = rlevel;
			}

			Node<T>* n = create_node(key, rlevel);

			//Insert node by rearranging pointers
			for (int i = 0; i <= rlevel; i++)
			{
				n->forward[i] = update[i]->forward[i];
				update[i]->forward[i] = n;
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
};

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

//TODO:
/*
-Structure to capture x percentile range
-calculate on the fly
-Implement a skip list
*/

int main()
{
//	generate_sample_data(128000000);
	SkipList<double> sl(3, 0.5);

	sl.insert_element(3);
	sl.insert_element(3);
	sl.insert_element(3);
	sl.insert_element(3);
	sl.insert_element(3);
	sl.insert_element(19);
	sl.insert_element(17);
	sl.insert_element(26);
	sl.insert_element(21);
	sl.insert_element(25);
	sl.display_list();

	return 0;
}