#include "intcode.hpp"

enum class intcode_base::mode : std::uint8_t { POS = 0, IMM = 1 };

enum class intcode_base::action : std::uint8_t {
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

intcode_base::intcode_base(intcode_base::program_type const program) noexcept
    : _program(std::move(program))
    , _ip(0)
{
}

intcode_base::~intcode_base() = default;

intcode_base::program_type intcode_base::parse_program(std::string const& in)
{
	program_type v;
	v.reserve(in.size() / 2);

	for (auto start = in.data(), end = start + in.size(); start < end;) {
		value_type result;
		start = (std::from_chars(start, end, result).ptr + 1);
		v.emplace_back(result);
	}

	return v;
}

constexpr intcode_base::action intcode_base::get_action(intcode_base::value_type const opcode) noexcept
{
	return static_cast<intcode_base::action>(opcode % 100);
}

constexpr intcode_base::value_type intcode_base::get_parameter(std::size_t const pos, mode const m) const noexcept
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

void intcode_base::binary_op(value_type (*const op)(value_type const, value_type const)) noexcept
{
	auto const p1 = get_parameter(_ip + 1, get_parameter_mode<1>(_program[_ip]));
	auto const p2 = get_parameter(_ip + 2, get_parameter_mode<2>(_program[_ip]));

	_program[_program[_ip + 3]] = op(p1, p2);

	_ip += 4;
}

void intcode_base::input()
{
	_program[_program[_ip + 1]] = input_handler();
	_ip += 2;
}

void intcode_base::output()
{
	output_handler(_program[_program[_ip + 1]]);
	_ip += 2;
}

void intcode_base::jump(bool (*const cond)(value_type const)) noexcept
{
	auto const p1 = get_parameter(_ip + 1, get_parameter_mode<1>(_program[_ip]));
	auto const p2 = get_parameter(_ip + 2, get_parameter_mode<2>(_program[_ip]));

	if (cond(p1))
		_ip = p2;
	else
		_ip += 3;
}

void intcode_base::insert(intcode_base::size_type const pos, intcode_base::value_type const value) noexcept
{
	_program[pos] = value;
}

intcode_base::value_type intcode_base::peek(size_type const pos) const noexcept
{
	return _program[pos];
}

void intcode_base::run()
{
	while (true) {
		auto const opcode = _program[_ip];
		auto const action = get_action(opcode);

		switch (action) {
		case action::ADD:
			binary_op([](value_type const lhs, value_type const rhs) { return lhs + rhs; });
			break;
		case action::MUL:
			binary_op([](value_type const lhs, value_type const rhs) { return lhs * rhs; });
			break;
		case action::IN:
			input();
			break;
		case action::OUT:
			output();
			break;
		case action::LT:
			binary_op([](value_type const lhs, value_type const rhs) { return value_type(lhs < rhs); });
			break;
		case action::EQ:
			binary_op([](value_type const lhs, value_type const rhs) { return value_type(lhs == rhs); });
			break;
		case action::JMPT:
			jump([](value_type const x) { return x != 0; });
			break;
		case action::JMPF:
			jump([](value_type const x) { return x == 0; });
			break;
		case action::HALT:
			return;
		}
	}
}

interactive_intcode::interactive_intcode(interactive_intcode::program_type program) noexcept
    : intcode_base(std::move(program))
{
}

interactive_intcode interactive_intcode::from_string(const std::string& str)
{
	return interactive_intcode(parse_program(str));
}

interactive_intcode::value_type interactive_intcode::input_handler()
{
	std::cout << "Input: " << std::flush;

	value_type input;
	std::cin >> input;

	return input;
}

void interactive_intcode::output_handler(interactive_intcode::value_type const output)
{
	std::cout << output << '\n';
}
