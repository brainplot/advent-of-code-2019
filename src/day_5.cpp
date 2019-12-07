#include "common.hpp"

enum class action : std::uint8_t { ADD = 1, MUL = 2, IN = 3, OUT = 4, JMPT = 5, JMPF = 6, LT = 7, EQ = 8, HALT = 99 };
enum class mode : std::uint8_t { POS = 0, IMM = 1 };

auto parse_program(std::string const& in)
{
	std::vector<int> v;
	v.reserve(in.size() / 2);

	for (auto start = in.data(), end = start + in.size(); start < end;) {
		int result;
		start = std::from_chars(start, end, result).ptr + 1;
		v.emplace_back(result);
	}

	return v;
}

constexpr auto get_action(int const opcode) noexcept
{
	return static_cast<action>(opcode % 100);
}

constexpr auto get_mode_1st_parameter(int const opcode) noexcept
{
	return static_cast<mode>(opcode / 100 % 10);
}

constexpr auto get_mode_2nd_parameter(int const opcode) noexcept
{
	return static_cast<mode>(opcode / 1000 % 10);
}

constexpr auto get_mode_3rd_parameter(int const opcode) noexcept
{
	return static_cast<mode>(opcode / 10000 % 10);
}

constexpr auto get_parameter(std::vector<int> const& program, std::size_t const ip, mode const m)
{
	using namespace std::string_literals;

	int res = 0;

	switch (m) {
	case mode::IMM:
		res = program[ip];
		break;
	case mode::POS:
		res = program[program[ip]];
		break;
	default:
		throw std::logic_error("Hit logic error. ip = "s += ip);
	}

	return res;
}

void handle_binary_op(std::vector<int>& program, std::size_t& ip, int (*const op)(int const, int const))
{
	auto const p1 = get_parameter(program, ip + 1, get_mode_1st_parameter(program[ip]));
	auto const p2 = get_parameter(program, ip + 2, get_mode_2nd_parameter(program[ip]));

	program[program[ip + 3]] = op(p1, p2);

	ip += 4;
}

void handle_in(std::vector<int>& program, std::size_t& ip)
{
	std::cout << "Input: " << std::flush;

	int input;
	std::cin >> input;

	program[program[ip + 1]] = input;

	ip += 2;
}

void handle_out(std::vector<int> const& program, std::size_t& ip)
{
	std::cout << program[program[ip + 1]] << '\n';
	ip += 2;
}

void handle_jump(std::vector<int> const& program, std::size_t& ip, bool (*const cond)(int))
{
	auto const p1 = get_parameter(program, ip + 1, get_mode_1st_parameter(program[ip]));
	auto const p2 = get_parameter(program, ip + 2, get_mode_2nd_parameter(program[ip]));

	if (cond(p1))
		ip = p2;
	else
		ip += 3;
}

void run(std::vector<int>& program)
{
	for (std::size_t ip = 0; ip < program.size();) {
		auto const opcode = program[ip];
		auto const action = get_action(opcode);

		switch (action) {
		case action::ADD:
			handle_binary_op(program, ip, [](int const lhs, int const rhs) { return lhs + rhs; });
			break;
		case action::MUL:
			handle_binary_op(program, ip, [](int const lhs, int const rhs) { return lhs * rhs; });
			break;
		case action::IN:
			handle_in(program, ip);
			break;
		case action::OUT:
			handle_out(program, ip);
			break;
		case action::LT:
			handle_binary_op(program, ip, [](int const lhs, int const rhs) { return int(lhs < rhs); });
			break;
		case action::EQ:
			handle_binary_op(program, ip, [](int const lhs, int const rhs) { return int(lhs == rhs); });
			break;
		case action::JMPT:
			handle_jump(program, ip, [](int const x) { return x != 0; });
			break;
		case action::JMPF:
			handle_jump(program, ip, [](int const x) { return x == 0; });
			break;
		case action::HALT:
			return;
		}
	}
}

int main(int const argc, char const** const argv)
{
	auto program = parse_program(load_file(boostrap(argc, argv)));
	run(program);
}
