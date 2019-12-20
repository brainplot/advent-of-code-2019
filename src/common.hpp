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

void trim_left(std::string& str) noexcept;

void trim_right(std::string& str) noexcept;

void trim(std::string& str) noexcept;

template<class Unit, class Function, class... Args>
void time_execution(Function const& f, Args&&... args) noexcept(noexcept(f(std::forward<Args>(args)...)))
{
	using namespace std::chrono;

	char const* unit;

	if constexpr (std::is_same_v<Unit, nanoseconds>)
		unit = "ms";
	else if constexpr (std::is_same_v<Unit, microseconds>)
		unit = "\u00b5s";
	else if constexpr (std::is_same_v<Unit, milliseconds>)
		unit = "ms";
	else if constexpr (std::is_same_v<Unit, seconds>)
		unit = "s";
	else if constexpr (std::is_same_v<Unit, minutes>)
		unit = "mins";
	else if constexpr (std::is_same_v<Unit, hours>)
		unit = "hrs";

	auto const start = steady_clock::now();
	f(std::forward<Args>(args)...);
	auto const end = steady_clock::now();

	std::cout << "Elapsed: " << duration_cast<Unit>(end - start).count() << unit << '\n';
}

#endif // SRC_COMMON_HPP
