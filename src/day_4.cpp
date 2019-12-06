#include "common.hpp"

#include <array>
#include <cstdint>

constexpr std::uint8_t nth_digit(std::uint32_t const number, std::uint8_t digit) noexcept
{
	std::uint32_t mask = 1;

	while (digit--)
		mask *= 10;

	return number / mask % 10;
}

constexpr bool has_only_two_adjacent_digits(std::array<std::uint8_t, 6> const& digits) noexcept
{
	auto previous_digit = digits.front();
	std::uint8_t count = 1;

	for (std::uint8_t i = 1; i < digits.size(); ++i) {
		if (digits[i] == previous_digit)
			++count;
		else if (count == 2)
			break;
		else
			count = 1;
		previous_digit = digits[i];
	}

	return count == 2;
}

bool has_only_increasing_digits(std::array<std::uint8_t, 6> const& digits) noexcept
{
	return std::is_sorted(digits.crbegin(), digits.crend());
}

bool meets_criteria(std::array<std::uint8_t, 6> const& digits) noexcept
{
	return has_only_two_adjacent_digits(digits) && has_only_increasing_digits(digits);
}

int main()
{
	std::uint32_t possible_combinations = 0;

	for (std::uint32_t i = 256310; i <= 732736; ++i) {
		// clang-format off
		std::array const digits{nth_digit(i, 0), nth_digit(i, 1), nth_digit(i, 2),
		                        nth_digit(i, 3), nth_digit(i, 4), nth_digit(i, 5)};
		// clang-format on

		if (meets_criteria(digits))
			++possible_combinations;
	}

	std::cout << possible_combinations << '\n';
}
