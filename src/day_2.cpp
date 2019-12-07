#include "common.hpp"

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <system_error>

std::vector<unsigned> get_program(std::string const& in);
void run_program(std::vector<unsigned>& p);
void print_result(std::uint8_t, std::uint8_t);

int main(int const argc, char const** const argv)
{
	auto const program = get_program(load_file(bootstrap(argc, argv)));

	for (std::uint8_t i = 0; i < 100; ++i) {
		for (std::uint8_t j = 0; j < 100; ++j) {
			auto program_copy{program};
			program_copy[1] = i;
			program_copy[2] = j;
			run_program(program_copy);
			if (program_copy.front() == 19690720) {
				print_result(i, j);
				break;
			}
		}
	}
}

std::vector<unsigned> get_program(std::string const& in)
{
	auto const end = in.data() + in.size();
	std::vector<unsigned> v;

	unsigned result;
	for (auto begin = in.data(); begin < end;) {
		auto const [ptr, ec] = std::from_chars(begin, end, result);
		if (ec == std::errc())
			v.emplace_back(result);
		begin = ptr + 1;
	}

	return v;
}

void run_program(std::vector<unsigned>& prog)
{
	constexpr unsigned ADD = 1;
	constexpr unsigned MUL = 2;
	constexpr unsigned HALT = 99;

	for (decltype(prog.size()) pc = 0; prog[pc] != HALT; pc += 4) {
		auto const first_op = prog[prog[pc + 1]];
		auto const second_op = prog[prog[pc + 2]];
		auto& result = prog[prog[pc + 3]];

		switch (prog[pc]) {
		case ADD:
			result = first_op + second_op;
			break;
		case MUL:
			result = first_op * second_op;
			break;
		}
	}
}

void print_result(std::uint8_t const i, std::uint8_t const j)
{
	std::cout << 100 * i + j << '\n';
}
