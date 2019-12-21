#include "common.hpp"

#include <array>
#include <iterator>

namespace {
std::size_t constexpr LAYER_WIDTH = 25u;
std::size_t constexpr LAYER_HEIGHT = 6u;
std::size_t constexpr LAYER_SIZE = LAYER_WIDTH * LAYER_HEIGHT;
} // namespace

std::string_view find_layer_with_fewest_zeros(std::vector<std::string_view> const& layers)
{
	auto min_zeros =
	    std::numeric_limits<std::iterator_traits<std::string_view::const_iterator>::difference_type>::max();

	std::string_view res;

	for (auto const layer : layers) {
		if (auto const zeros = std::count(layer.cbegin(), layer.cend(), '0'); zeros < min_zeros) {
			min_zeros = zeros;
			res = layer;
		}
	}

	return res;
}

std::size_t ones_by_twos(std::string_view const layer) noexcept
{
	std::size_t ones = 0;
	std::size_t twos = 0;

	for (auto const ch : layer) {
		if (ch == '1')
			++ones;
		else if (ch == '2')
			++twos;
	}

	return ones * twos;
}

std::vector<std::string_view> get_layers(std::string_view input)
{
	std::vector<std::string_view> layers;

	while (!input.empty()) {
		layers.push_back(input.substr(0, LAYER_SIZE));
		input.remove_prefix(LAYER_SIZE);
	}

	return layers;
}

void part_one(std::vector<std::string_view> const& layers)
{
	std::cout << ones_by_twos(find_layer_with_fewest_zeros(layers)) << '\n';
}

auto final_layer(std::vector<std::string_view> const& layers) noexcept
{
	std::array<char, LAYER_SIZE> final_layer;

	for (decltype(final_layer)::size_type i = 0; i < final_layer.size(); ++i) {
		for (auto const layer : layers)
			if (layer[i] != '2') {
				final_layer[i] = layer[i];
				break;
			}
	}

	return final_layer;
}

void part_two(std::vector<std::string_view> const& layers) noexcept
{
	auto const final_picture = final_layer(layers);
	for (std::remove_reference_t<decltype(final_picture)>::size_type h = 0; h < LAYER_HEIGHT; ++h) {
		for (auto w = LAYER_WIDTH * h, end = w + LAYER_WIDTH; w < end; ++w)
			std::cout << (final_picture[w] == '1' ? '#' : ' ');
		std::cout << '\n';
	}
}

int main(int const argc, char const** const argv)
{
	auto input = load_file(bootstrap(argc, argv));
	trim(input);

	auto const layers = get_layers(input);
	part_one(layers);
	part_two(layers);
}
