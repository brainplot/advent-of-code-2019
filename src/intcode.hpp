#ifndef SRC_INTCODE_HPP
#define SRC_INTCODE_HPP

#include "common.hpp"

class intcode_base {
public:
	using program_type = std::vector<long>;
	using value_type = program_type::value_type;
	using size_type = program_type::size_type;

	void run();
	void insert(size_type pos, value_type value) noexcept;
	value_type peek(size_type pos = 0) const noexcept;

private:
	enum class mode : std::uint8_t { POS = 0, IMM = 1, REL = 2 };

	enum class action : std::uint8_t {
		ADD = 1,
		MUL = 2,
		IN = 3,
		OUT = 4,
		JMPT = 5,
		JMPF = 6,
		LT = 7,
		EQ = 8,
		SRB = 9,
		HALT = 99
	};

	value_type get_parameter(size_type pos, mode m) noexcept;

	void binary_op(value_type (*op)(value_type, value_type)) noexcept;
	void input();
	void output();
	void jump(bool (*cond)(value_type)) noexcept;
	void set_relative_base() noexcept;

	virtual value_type input_handler() = 0;
	virtual void output_handler(value_type data) = 0;

	action get_action() const noexcept;

	void maybe_resize(size_type pos);

	value_type get_parameter_helper(value_type const the_operand, mode const the_mode) noexcept;
	value_type& get_result_location_helper(value_type const the_operand, mode const the_mode) noexcept;

	template<std::uint8_t N>
	inline mode get_parameter_mode() const noexcept
	{
		static_assert(1 <= N && N <= 3, "Each opcode has at most 3 parameters");

		auto const opcode = current_opcode();
		// clang-format off
		     if constexpr (N == 1) return static_cast<mode>(opcode / 100 % 10);
		else if constexpr (N == 2) return static_cast<mode>(opcode / 1000 % 10);
		else if constexpr (N == 3) return static_cast<mode>(opcode / 10000 % 10);
		// clang-format on
	}

	template<std::uint8_t N>
	inline value_type get_parameter() noexcept
	{
		static_assert(N == 1 || N == 2, "Each opcode has at most 2 parameters");

		auto const operand = _program[_ip + N];
		auto const mode = get_parameter_mode<N>();

		return get_parameter_helper(operand, mode);
	}

	template<std::uint8_t N>
	inline value_type& get_result_location() noexcept
	{
		auto const operand = _program[_ip + N];
		auto const mode = get_parameter_mode<N>();

		return get_result_location_helper(operand, mode);
	}

	inline value_type current_opcode() const noexcept
	{
		return _program[_ip];
	}

protected:
	explicit intcode_base(program_type program) noexcept;
	virtual ~intcode_base();

	static program_type parse_program(std::string const& in);

	program_type _program;
	size_type _ip;
	size_type _rb;
};

class interactive_intcode : public intcode_base {
public:
	static interactive_intcode from_string(std::string const& str);

private:
	interactive_intcode(program_type program) noexcept;

	value_type input_handler() override;
	void output_handler(value_type output) override;
};

#endif // SRC_INTCODE_HPP
