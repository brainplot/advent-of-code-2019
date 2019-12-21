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

#if defined(_MSC_VER)
#define DISABLE_WARNING_PUSH __pragma(warning(push))
#define DISABLE_WARNING_POP __pragma(warning(pop))
#define DISABLE_WARNING(warning_number) __pragma(warning(disable : warning_number))

#define DISABLE_WARNING_POSSIBLE_LOSS_OF_DATA DISABLE_WARNING(4244)

#elif defined(__GNUC__) || defined(__clang__)
#define DO_PRAGMA(X) _Pragma(#X)
#define DISABLE_WARNING_PUSH DO_PRAGMA(GCC diagnostic push)
#define DISABLE_WARNING_POP DO_PRAGMA(GCC diagnostic pop)
#define DISABLE_WARNING(warning_name) DO_PRAGMA(GCC diagnostic ignored #warning_name)

#define DISABLE_WARNING_POSSIBLE_LOSS_OF_DATA DISABLE_WARNING(-Wfloat-conversion)

#else
#define DISABLE_WARNING_PUSH
#define DISABLE_WARNING_POP
#define DISABLE_WARNING_POSSIBLE_LOSS_OF_DATA

#endif

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
