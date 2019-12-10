#include "intcode.hpp"

void part_one(monotask_intcode_computer program)
{
	program.insert(1, 12);
	program.insert(2, 2);
	program.run();
	std::cout << program.peek() << '\n';
}

void part_two(monotask_intcode_computer const& program, monotask_intcode_computer::value_type const target)
{
	auto found = false;

	for (std::uint8_t i = 0; !found && i < 100; ++i) {
		for (std::uint8_t j = 0; j < 100; ++j) {
			auto backup{program};
			backup.insert(1, i);
			backup.insert(2, j);
			backup.run();
			if (backup.peek() == target) {
				found = true;
				std::cout << 100 * i + j << '\n';
				break;
			}
		}
	}
}

int main(int const argc, char const** const argv)
{
	auto program = monotask_intcode_computer::from_string(load_file(bootstrap(argc, argv)));

	part_one(program);
	part_two(program, 19690720);
}
