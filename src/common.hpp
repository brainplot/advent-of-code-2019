#ifndef SRC_COMMON_HPP
#define SRC_COMMON_HPP

#include <algorithm>
#include <charconv>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

std::ifstream bootstrap(int, char const**);

std::string load_file(std::ifstream in);

std::vector<std::string_view> split(std::string_view, char const*);

template<class Unit, class Function>
void time_execution(Function const& f) noexcept(noexcept(f))
{
	using namespace std::chrono;

	char const* unit;

	if constexpr (std::is_same_v<Unit, nanoseconds>)
		unit = "ms";
	else if (std::is_same_v<Unit, microseconds>)
		unit = "\u00b5s";
	else if (std::is_same_v<Unit, milliseconds>)
		unit = "ms";
	else if (std::is_same_v<Unit, seconds>)
		unit = "s";
	else if (std::is_same_v<Unit, minutes>)
		unit = "mins";
	else if (std::is_same_v<Unit, hours>)
		unit = "hrs";

	auto const start = steady_clock::now();
	f();
	auto const end = steady_clock::now();

	std::cout << "Elapsed: " << duration_cast<Unit>(end - start).count() << unit << '\n';
}

#endif // SRC_COMMON_HPP
