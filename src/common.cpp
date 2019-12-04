#include "common.hpp"

#include <sstream>

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
	std::ostringstream oss;
	oss << in.rdbuf();

	return oss.str();
}

std::vector<std::string_view> split(std::string_view input, char const delimiter)
{
	std::vector<std::string_view> v;

	std::string_view::size_type found_at = 0;
	while (true) {
		found_at = input.find(delimiter);
		v.emplace_back(input.substr(0, found_at));
		if (found_at != std::string::npos)
			input.remove_prefix(found_at + 1);
		else
			break;
	}

	return v;
}
