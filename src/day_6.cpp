#include "common.hpp"

#include <cmath>
#include <unordered_map>
#include <unordered_set>

template<class Inserter>
auto make_map_helper(std::string_view const input, Inserter inserter)
{
	using sv = std::string_view;

	for (sv::size_type i = 0, separator; (separator = input.find(')', i)) != sv::npos;) {
		auto const orbitee_len = separator - i;
		auto const new_line = input.find('\n', ++separator);
		auto const orbiter_len = new_line - separator;
		inserter(input.substr(i, orbitee_len), input.substr(separator, orbiter_len));
		i = new_line + 1;
	}
}

auto make_map_orbitee_to_orbiters(std::string_view const input)
{
	using sv = std::string_view;

	std::unordered_multimap<sv, sv> orbits;

	DISABLE_WARNING_PUSH
	DISABLE_WARNING_POSSIBLE_LOSS_OF_DATA
	orbits.reserve(std::sqrt(input.size()));
	DISABLE_WARNING_POP
	make_map_helper(input, [&](sv const orbitee, sv const orbiter) { orbits.emplace(orbitee, orbiter); });

	return orbits;
}

auto make_map_orbiter_to_orbitee(std::string_view const input)
{
	using sv = std::string_view;

	std::unordered_map<sv, sv> orbits;

	DISABLE_WARNING_PUSH
	DISABLE_WARNING_POSSIBLE_LOSS_OF_DATA
	orbits.reserve(std::sqrt(input.size()));
	DISABLE_WARNING_POP
	make_map_helper(input, [&](sv const orbitee, sv const orbiter) { orbits.emplace(orbiter, orbitee); });

	return orbits;
}

void count_indirect_orbits_helper(std::unordered_multimap<std::string_view, std::string_view> const& orbits,
                                  std::size_t& counter,
                                  std::string_view const start = "COM",
                                  std::size_t level = 0)
{
	counter += level;
	for (auto [it, end] = orbits.equal_range(start); it != end; ++it) {
		count_indirect_orbits_helper(orbits, counter, it->second, level + 1);
	}
}

std::size_t count_indirect_orbits(std::unordered_multimap<std::string_view, std::string_view> const& orbits)
{
	std::size_t counter = 0;
	count_indirect_orbits_helper(orbits, counter);

	return counter;
}

void part_one(std::unordered_multimap<std::string_view, std::string_view> const& orbits)
{
	std::cout << "Total number of indirect orbits: " << count_indirect_orbits(orbits) << '\n';
}

auto make_path_from_you_to_com(std::unordered_map<std::string_view, std::string_view> const& orbits)
{
	std::unordered_set<std::string_view> common_path;

	for (auto it = orbits.find("YOU"); it != orbits.cend(); it = orbits.find(it->second))
		common_path.emplace(it->second);

	return common_path;
}

void remove_nodes_from_common_ancestor_to_com(std::unordered_map<std::string_view, std::string_view> const& orbits,
                                              std::unordered_set<std::string_view>& path) noexcept
{
	for (auto it = orbits.find("SAN"); it != orbits.cend(); it = orbits.find(it->second))
		if (path.count(it->second))
			path.erase(it->second);
		else
			path.emplace(it->second);
}

void part_two(std::unordered_map<std::string_view, std::string_view> const& orbits)
{
	auto common_path = make_path_from_you_to_com(orbits);
	remove_nodes_from_common_ancestor_to_com(orbits, common_path);
	std::cout << "Minimum number of orbital transfers: " << common_path.size() << '\n';
}

int main(int const argc, char const** const argv)
{
	auto const input_str = load_file(bootstrap(argc, argv));

	part_one(make_map_orbitee_to_orbiters(input_str));
	part_two(make_map_orbiter_to_orbitee(input_str));
}
