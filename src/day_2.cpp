#include "intcode.hpp"

class intcode_computer2 : public intcode_computer {
public:
	static intcode_computer2 from_string(std::string const& in)
	{
		return intcode_computer2(parse_program(in));
	}

	value_type run(std::initializer_list<std::pair<size_type, value_type>> const& replacements,
	               size_type result_location = 0)
	{
		for (auto const [index, value] : replacements)
			_program[index] = value;

		intcode_computer::run();

		return _program[result_location];
	}

private:
	intcode_computer2(program_type program) noexcept
	    : intcode_computer(std::move(program))
	{
	}
};

void print_result(std::uint8_t const n1, std::uint8_t const n2)
{
	std::cout << 100 * n1 + n2 << '\n';
}

void part_one(intcode_computer2 program)
{
	std::cout << program.run({{1, 12}, {2, 2}}) << '\n';
}

void part_two(intcode_computer2 const& program, intcode_computer2::value_type const target)
{
	for (std::uint8_t i = 0; i < 100; ++i) {
		for (std::uint8_t j = 0; j < 100; ++j) {
			auto program_copy{program};
			auto const result = program_copy.run({{1, i}, {2, j}});
			if (result == target) {
				print_result(i, j);
				break;
			}
		}
	}
}

int main(int const argc, char const** const argv)
{
	auto program = intcode_computer2::from_string(load_file(bootstrap(argc, argv)));

	part_one(program);
	part_two(program, 19690720);
}
