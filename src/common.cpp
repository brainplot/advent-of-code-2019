#include "common.hpp"

std::ifstream boostrap(int const argc, char const** const argv)
{
	if (argc < 2) {
		std::cerr << "Not enough arguments.\nPlease supply input file.\n";
		std::exit(EXIT_FAILURE);
	}

	std::ifstream in(argv[1]);
	if (!in) {
		std::cerr << "Error while opening " << argv[1] << '\n';
		std::exit(EXIT_FAILURE);
	}

	return in;
}

std::string load_file(std::ifstream in)
{
	std::string program;
	std::getline(in, program);

	return program;
}
