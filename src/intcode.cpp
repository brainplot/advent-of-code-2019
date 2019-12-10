#include "intcode.hpp"

enum class intcode_computer_base::mode : std::uint8_t { POS = 0, IMM = 1 };

enum class intcode_computer_base::action : std::uint8_t {
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

intcode_computer_base::intcode_computer_base(intcode_computer_base::program_type* const program) noexcept
    : _current_program(program)
    , _ip(0)
{
}

intcode_computer_base::~intcode_computer_base() = default;

intcode_computer_base::program_type intcode_computer_base::parse_program(std::string const& in)
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

constexpr intcode_computer_base::action
intcode_computer_base::get_action(intcode_computer_base::value_type const opcode) noexcept
{
	return static_cast<intcode_computer_base::action>(opcode % 100);
}

constexpr intcode_computer_base::value_type intcode_computer_base::get_parameter(std::size_t const pos,
                                                                                 mode const m) const noexcept
{
	auto const& program = *_current_program;
	value_type res = 0;

	switch (m) {
	case mode::IMM:
		res = program[pos];
		break;
	case mode::POS:
		res = program[program[pos]];
		break;
	}

	return res;
}

void intcode_computer_base::handle_binary_op(value_type (*const op)(value_type const, value_type const)) noexcept
{
	auto& program = *_current_program;
	auto const p1 = get_parameter(_ip + 1, get_parameter_mode<1>(program[_ip]));
	auto const p2 = get_parameter(_ip + 2, get_parameter_mode<2>(program[_ip]));

	program[program[_ip + 3]] = op(p1, p2);

	_ip += 4;
}

void intcode_computer_base::handle_in()
{
	auto& program = *_current_program;
	program[program[_ip + 1]] = input_handler();
}

intcode_computer_base::value_type intcode_computer_base::input_handler()
{
	std::cout << "Input: " << std::flush;

	value_type input;
	std::cin >> input;

	_ip += 2;

	return input;
}

void intcode_computer_base::handle_out()
{
	auto const& program = *_current_program;
	output_handler(program[program[_ip + 1]]);
}

void intcode_computer_base::output_handler(intcode_computer_base::value_type const data)
{
	std::cout << data << '\n';
	_ip += 2;
}

void intcode_computer_base::handle_jump(bool (*const cond)(value_type const)) noexcept
{
	auto const& program = *_current_program;
	auto const p1 = get_parameter(_ip + 1, get_parameter_mode<1>(program[_ip]));
	auto const p2 = get_parameter(_ip + 2, get_parameter_mode<2>(program[_ip]));

	if (cond(p1))
		_ip = p2;
	else
		_ip += 3;
}

void intcode_computer_base::insert(intcode_computer_base::size_type const pos,
                                   intcode_computer_base::value_type const value) noexcept
{
	(*_current_program)[pos] = value;
}

intcode_computer_base::value_type intcode_computer_base::peek(size_type const pos) const noexcept
{
	return (*_current_program)[pos];
}

void intcode_computer_base::run()
{
	auto const& program = *_current_program;

	while (true) {
		auto const opcode = program[_ip];
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

monotask_intcode_computer::monotask_intcode_computer(monotask_intcode_computer::program_type program) noexcept
    : intcode_computer_base(&_program)
    , _program(std::move(program))
{
}

monotask_intcode_computer::monotask_intcode_computer(monotask_intcode_computer const& other)
: intcode_computer_base(&_program)
, _program(other._program)
{}

monotask_intcode_computer& monotask_intcode_computer::operator=(monotask_intcode_computer const& other)
{
	_program = other._program;
	_current_program = &_program;
	_ip = other._ip;

	return *this;
}

monotask_intcode_computer monotask_intcode_computer::from_string(const std::string& str)
{
	return monotask_intcode_computer(parse_program(str));
}
