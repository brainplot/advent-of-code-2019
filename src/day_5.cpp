#include "intcode.hpp"

int main(int const argc, char const** const argv)
{
	auto icomputer = monotask_intcode_computer::from_string(load_file(bootstrap(argc, argv)));
	icomputer.run();
}
