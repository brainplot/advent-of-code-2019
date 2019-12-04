#ifndef SRC_COMMON_HPP
#define SRC_COMMON_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::ifstream boostrap(int, char const**);

std::string load_file(std::ifstream in);

#endif // SRC_COMMON_HPP
