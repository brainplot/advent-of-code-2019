#include "intcode.hpp"

enum class intcode_computer::mode : std::uint8_t { POS = 0, IMM = 1 };

enum class intcode_computer::action : std::uint8_t {
	ADD = 1,
	MUL = 2,
	IN = 3,
	OUT = 4,
	JMPT = 5,
	JMPF = 6,
	LT = 7,
	EQ = 8,
	HALT = 99
};

intcode_computer::intcode_computer(intcode_computer::program_type program) noexcept
    : _program(std::move(program))
    , _ip(0)
{
}

intcode_computer::program_type intcode_computer::parse_program(std::string const& in)
{
	program_type v;
	v.reserve(in.size() / 2);

	for (auto start = in.data(), end = start + in.size(); start < end;) {
		value_type result;
		start = std::from_chars(start, end, result).ptr + 1;
		v.emplace_back(result);
	}

	return v;
}

intcode_computer intcode_computer::from_string(std::string const& in)
{
	return intcode_computer(parse_program(in));
}

constexpr intcode_computer::action intcode_computer::get_action(intcode_computer::value_type const opcode) noexcept
{
	return static_cast<intcode_computer::action>(opcode % 100);
}

constexpr intcode_computer::value_type intcode_computer::get_parameter(std::size_t const pos, mode const m) const
    noexcept
{
	value_type res = 0;

	switch (m) {
	case mode::IMM:
		res = _program[pos];
		break;
	case mode::POS:
		res = _program[_program[pos]];
		break;
	}

	return res;
}

void intcode_computer::handle_binary_op(value_type (*const op)(value_type const, value_type const)) noexcept
{
	auto const p1 = get_parameter(_ip + 1, get_parameter_mode<1>(_program[_ip]));
	auto const p2 = get_parameter(_ip + 2, get_parameter_mode<2>(_program[_ip]));

	_program[_program[_ip + 3]] = op(p1, p2);

	_ip += 4;
}

void intcode_computer::handle_in()
{
	std::cout << "Input: " << std::flush;

	value_type input;
	std::cin >> input;

	_program[_program[_ip + 1]] = input;

	_ip += 2;
}

void intcode_computer::handle_out()
{
	std::cout << _program[_program[_ip + 1]] << '\n';
	_ip += 2;
}

void intcode_computer::handle_jump(bool (*const cond)(value_type const)) noexcept
{
	auto const p1 = get_parameter(_ip + 1, get_parameter_mode<1>(_program[_ip]));
	auto const p2 = get_parameter(_ip + 2, get_parameter_mode<2>(_program[_ip]));

	if (cond(p1))
		_ip = p2;
	else
		_ip += 3;
}

void intcode_computer::run()
{
	while (true) {
		auto const opcode = _program[_ip];
		auto const action = get_action(opcode);

		switch (action) {
		case action::ADD:
			handle_binary_op([](value_type const lhs, value_type const rhs) { return lhs + rhs; });
			break;
		case action::MUL:
			handle_binary_op([](value_type const lhs, value_type const rhs) { return lhs * rhs; });
			break;
		case action::IN:
			handle_in();
			break;
		case action::OUT:
			handle_out();
			break;
		case action::LT:
			handle_binary_op([](value_type const lhs, value_type const rhs) { return value_type(lhs < rhs); });
			break;
		case action::EQ:
			handle_binary_op([](value_type const lhs, value_type const rhs) { return value_type(lhs == rhs); });
			break;
		case action::JMPT:
			handle_jump([](value_type const x) { return x != 0; });
			break;
		case action::JMPF:
			handle_jump([](value_type const x) { return x == 0; });
			break;
		case action::HALT:
			return;
		}
	}
}
