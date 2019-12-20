#include "intcode.hpp"

intcode_base::intcode_base(intcode_base::program_type const program) noexcept
    : _program(std::move(program))
    , _ip(0)
    , _rb(0)
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

intcode_base::action intcode_base::get_action() const noexcept
{
	return static_cast<intcode_base::action>(current_opcode() % 100);
}

void intcode_base::maybe_resize(size_type const pos)
{
	std::ptrdiff_t const diff = pos - _program.size();
	if (diff > 0)
		_program.resize(_program.size() + diff + 2); // poor man's geometric resizing
}

intcode_base::value_type intcode_base::get_parameter_helper(value_type const the_operand, mode const the_mode) noexcept
{
	value_type res = std::numeric_limits<value_type>::max(); // sentinel value

	switch (the_mode) {
	case mode::POS:
		maybe_resize(the_operand);
		res = _program[the_operand];
		break;
	case mode::IMM:
		res = the_operand;
		break;
	case mode::REL:
		auto const relative_pos = _rb + the_operand;
		maybe_resize(relative_pos);
		res = _program[relative_pos];
		break;
	}

	return res;
}

intcode_base::value_type& intcode_base::get_result_location_helper(intcode_base::value_type const the_operand,
                                                                   intcode_base::mode const the_mode) noexcept
{
	value_type* res;

	switch (the_mode) {
	case mode::POS:
		maybe_resize(the_operand);
		res = &_program[the_operand];
		break;
	case mode::REL: {
		auto const relative_pos = _rb + the_operand;
		maybe_resize(relative_pos);
		res = &_program[relative_pos];
		break;
	}
	default:
		res = nullptr; // this will cause a segfault which will let us know if there's any unhandled case
		std::cerr << __func__ << ": unhandled case (" << static_cast<int>(the_mode) << ")\n";
	}

	return *res;
}

void intcode_base::binary_op(value_type (*const op)(value_type const, value_type const)) noexcept
{
	auto const first = get_parameter<1>();
	auto const second = get_parameter<2>();
	auto& loc = get_result_location<3>();

	loc = op(first, second);
	_ip += 4;
}

void intcode_base::input()
{
	auto& loc = get_result_location<1>();

	loc = input_handler();
	_ip += 2;
}

void intcode_base::output()
{
	auto const param = get_parameter<1>();

	output_handler(param);
	_ip += 2;
}

void intcode_base::jump(bool (*const cond)(value_type const)) noexcept
{
	auto const first = get_parameter<1>();
	auto const second = get_parameter<2>();

	if (cond(first))
		_ip = second;
	else
		_ip += 3;
}

void intcode_base::set_relative_base() noexcept
{
	auto const param = get_parameter<1>();

	_rb += param;
	_ip += 2;
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
		switch (get_action()) {
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
			binary_op([](value_type const lhs, value_type const rhs) -> value_type { return lhs < rhs; });
			break;
		case action::EQ:
			binary_op([](value_type const lhs, value_type const rhs) -> value_type { return lhs == rhs; });
			break;
		case action::JMPT:
			jump([](value_type const x) { return x != 0; });
			break;
		case action::JMPF:
			jump([](value_type const x) { return x == 0; });
			break;
		case action::SRB:
			set_relative_base();
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
