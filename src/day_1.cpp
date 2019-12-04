#include "common.hpp"

#include <iterator>
#include <numeric>

using Field = unsigned long;

Field compute_fuel(Field) noexcept;

int main(int const argc, char const** const argv)
{
	auto input_file = boostrap(argc, argv);
	auto const res = std::accumulate(std::istream_iterator<Field>{input_file},
	                                 std::istream_iterator<Field>{},
	                                 Field(0),
	                                 [](Field const acc, Field const curr) { return acc + compute_fuel(curr); });

	std::cout << res << '\n';
}

Field compute_fuel(Field field) noexcept
{
	Field result = 0;

	while (true) {
		field /= 3;
		if (field > 1)
			field -= 2;
		else
			break;
		result += field;
	}

	return result;
}
