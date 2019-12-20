#include "intcode.hpp"

int main(int const argc, char const** const argv)
{
	auto icomputer = interactive_intcode::from_string(load_file(bootstrap(argc, argv)));
	icomputer.run();
}
