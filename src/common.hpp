#ifndef SRC_COMMON_HPP
#define SRC_COMMON_HPP

#include <algorithm>
#include <charconv>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

std::ifstream bootstrap(int, char const**);

std::string load_file(std::ifstream in);

std::vector<std::string_view> split(std::string_view, char const*);

#endif // SRC_COMMON_HPP
