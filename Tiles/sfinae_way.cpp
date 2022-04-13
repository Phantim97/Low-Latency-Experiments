#include <array>
template<int First, int Last>
struct static_for
{
    template<typename Fn>
    void operator()(Fn const& fn) const
    {
        if (First < Last)
        {
            fn(First);
            static_for<First + 1, Last>()(fn);
        }
    }
};

template <int N>
struct static_for<N, N>
{
    template<typename Fn>
    void operator()(Fn const& fn) const
    {}
};

#include <array>
constexpr std::array<> v = { 7,5,6,345,63,45,34,34,53,45,345,43 };

template<int n, int l>
struct Solver
{
    static const int value = static_for<n, v[l]>(Solver<n, l - 1>::value);
};

template<int n>
struct Solver<n, 0>
{
    static const int value = 1;
};

#include <array>
//Metaprogramming approach

int main()
{
    constexpr int sz = 1000000;

    static int count;

    static_for<0, sz>()([&](const int i)
        {
            count += Solver<10, 7>::value;
        });

    std::cout << count << '\n';

    return 0;
}
