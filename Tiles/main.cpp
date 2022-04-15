#include <chrono>
#include <iostream>
#include <vector>

#include "ScopedTimer.h"
#include <omp.h>

static std::vector<int> generate_strips()
{
    ScopedTimer<std::chrono::milliseconds> st;
    std::vector<int> strips;

    //We can represent the tiles as a binary 1 (start of tile and 0 rest of tile)

    //masks to generate strips
    constexpr int mask_000 = 0b111111111111111111111111111000;
    constexpr int mask_11 = 0b000000000000000000000000000011;
    constexpr int mask_all = 0b111111111111111111111111111111;
    constexpr int mask_first = 0b100000000000000000000000000000;
    constexpr int mask_last = 0b000000000000000000000000000001;

    //OpenMP reduces generation time from ~2305ms to ~605ms
	#pragma omp parallel for (shared : strips)
    for (int i = 0; i < mask_all + 1; ++i)
    {
        int temp = i;

        if (!(temp & mask_first)) //We need a tile in the first part for the stripe to be valid
        {
            continue;
        }
        if (temp & mask_last) //There are no tiles of size 1 so this is also an invalid region
        {
            continue;
        }

        int n = 0;
        for (; n < 29; ++n)
        {
            if ((temp | mask_000) == mask_000) //Invalid Tile arrangement found
            {
                break;
            }

            if ((temp & mask_11) == mask_11) //Invalid Tile arrangement found
            {
                break;
            }

            temp = temp >> 1;
        }

        if (n == 29) //If we make it this far we add the strip to the group
        {
            strips.push_back(i);
        }
    }

    return strips;
}

class TileLayout
{
private:
    const int strips_ = 0;
    std::vector<int> v_;
    std::vector<std::vector<int>> valid_;
	std::vector<std::vector<long long>> cache_; //store subproblems in a cache
    bool loaded_ = false;
	long long solve(int i, int height); //long long for rapid growth of answer numerically
    void set_valid();

public:
    explicit TileLayout(int s);
	explicit TileLayout(int s, const std::vector<int>& vec);
    void load(const std::vector<int>& vec);
    void run();
};

TileLayout::TileLayout(const int s) : strips_(s)
{
}

TileLayout::TileLayout(const int s, const std::vector<int>& vec) : strips_(s)
{
    this->load(vec); //If we are given vector at construction just run load
}

void TileLayout::load(const std::vector<int>& vec)
{
    if (!loaded_)
    {
        v_ = vec;
        for (std::size_t i = 0; i != v_.size(); ++i)
        {
            //Create intermediate subproblem storage
            cache_.emplace_back(strips_, -1);
        }

        this->set_valid();
        loaded_ = true;
    }
}

void TileLayout::run()
{
    //Benchmark the time to compute
    ScopedTimer<std::chrono::milliseconds> st;

    if (loaded_)
    {
        long long res = 0;
        for (size_t i = 0; i != v_.size(); ++i)
        {
            res += solve(i, strips_ - 1);
        }

        std::cout << "Number of Designs: " << res << '\n';
        std::cout << "Number of Designs: " << res << '\n';
    }
    else
    {
        std::cout << "Loading step not performed\n";
    }
}

long long TileLayout::solve(const int i, const int height)
{
    if (height == 0)
    {
        return 1;
    }

    if (height == 1)
    {
        return valid_[i].size();
    }

    long long res = cache_[i][height - 1];

    if (res != -1)
    {
        return res;
    }

    res = 0;

    for (const int n : valid_[i])
    {
        res += solve(n, height - 1);
    }

    return res;
}

void TileLayout::set_valid()
{
    for (std::size_t i = 0; i != v_.size(); i++)
    {
        std::vector<int> temp;
        for (std::size_t j = 0; j != v_.size(); j++)
        {
            if ((v_[i] & v_[j]) == 0b100000000000000000000000000000)
            {
                temp.push_back(j);
            }
        }

        valid_.push_back(temp);
    }
}

int main()
{
    constexpr size_t num_strips = 7;
    TileLayout t(num_strips, generate_strips());
    t.run();
    return 0;
}