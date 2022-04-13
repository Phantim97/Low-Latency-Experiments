#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cassert>
#include <string>

// $> ./fastdesigns 7 < stripes.txt

#define LOG std::cerr << __FUNCTION__ << std::endl

struct Designs
{
    Designs(int s);
    void load();
    void run();
    void solve(int i, int level);
    void set_compatible();
private:
    const int stripes_;
    std::vector<int> v_;
    std::vector<std::vector<int>> compatible_;
    std::size_t count_ = 0;
};

Designs::Designs(const int s) : stripes_(s)
{
    LOG;
}

void Designs::set_compatible()
{
    LOG;
    for (std::size_t i = 0; i != v_.size(); ++i)
    {
        std::vector<int> c;
        for (std::size_t n = 0; n != v_.size(); ++n)
        {
            if ((v_[i] & v_[n]) == 0b100000000000000000000000000000)
            {
                c.push_back(n);
            }
        }
        compatible_.push_back(c);
        //std::cerr << "the stripe = " << s << " is compatible with " << c.size() << " other stripes\n";
    }
    LOG;
}

void Designs::load()
{
    int n;
    while (std::cin >> n)
    {
        v_.push_back(n);
        std::string s;
        std::cin >> s;
    }
    std::cerr << "input size = " << v_.size() << "\n";
}

void Designs::solve(const int i, const int level)
{
    if (level == 0)
    {
        ++count_;
        return;
    }
    for (const int n : compatible_[i])
    {
        solve(n, level - 1);
    }
}

void Designs::run()
{
    LOG;
    for (size_t i = 0; i != v_.size(); ++i)
    {
        solve(i, stripes_ - 1);
    }
    std::cerr << "design count = " << count_ << "\n";
}

int main(const int argc, char* argv[])
{
	const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	const std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::cout << "start at " << std::ctime(&start_time);
    assert(argc == 2);
	const int stripes = std::stoi(argv[1]);
    Designs d(stripes);
    d.load();
    d.set_compatible();
    d.run();
	const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	const std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "start at " << std::ctime(&start_time);
    std::cout << "end at " << std::ctime(&end_time);
    std::cout << "elapsed " << elapsed_seconds.count() << " seconds for computing " << stripes << " stripes\n";
    return 0;
}