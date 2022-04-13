#pragma once
template<typename T>
struct st_numeric
{
	static const bool val = false;
};

//Full specialization for template class
template<>
struct st_numeric<int>
{
	static const bool val = true;
	using type = int;
};

template<>
struct st_numeric<short>
{
	static const bool val = true;
	using type = short;
};

template<>
struct st_numeric<long>
{
	static const bool val = true;
	using type = long;
};

template<>
struct st_numeric<long long>
{
	static const bool val = true;
	using type = long long;
};

//Unsigned

//Full specialization for template class
template<>
struct st_numeric<unsigned int>
{
	static const bool val = true;
	using type = unsigned int;
};

template<>
struct st_numeric<unsigned short>
{
	static const bool val = true;
	using type = unsigned short;
};

template<>
struct st_numeric<unsigned long>
{
	static const bool val = true;
	using type = unsigned long;
};

template<>
struct st_numeric<unsigned long long>
{
	static const bool val = true;
	using type = unsigned long long;
};

template<>
struct st_numeric<double>
{
	static const bool val = true;
	using type = double;
};

template<>
struct st_numeric<float>
{
	static const bool val = true;
	using type = float;
};

//value template using constexpr keyword
template<typename T>
constexpr bool is_numeric = st_numeric<T>::val; // compile time resolution

//using template - type alias
template<typename T>
using numeric_t = typename st_numeric<T>::type;