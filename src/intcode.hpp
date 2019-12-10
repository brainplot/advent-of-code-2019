#ifndef SRC_INTCODE_HPP
#define SRC_INTCODE_HPP

#include "common.hpp"

class intcode_computer_base {
public:
	using program_type = std::vector<int>;
	using value_type = program_type::value_type;
	using size_type = program_type::size_type;

	void run();
	void insert(size_type pos, value_type value) noexcept;
	value_type peek(size_type pos = 0) const noexcept;

private:
	enum class action : std::uint8_t;
	enum class mode : std::uint8_t;

	constexpr int get_parameter(size_type pos, mode m) const noexcept;

	void handle_binary_op(value_type (*op)(value_type, value_type)) noexcept;
	void handle_in();
	void handle_out();
	void handle_jump(bool (*cond)(value_type)) noexcept;

	virtual value_type input_handler();
	virtual void output_handler(value_type data);

	static constexpr action get_action(value_type opcode) noexcept;

	template<std::uint8_t N>
	inline static constexpr mode get_parameter_mode(value_type const opcode) noexcept
	{
		static_assert(1 <= N && N <= 3, "Each opcode has at most 3 parameters");

		// clang-format off
		     if constexpr (N == 1) return static_cast<mode>(opcode / 100 % 10);
		else if constexpr (N == 2) return static_cast<mode>(opcode / 1000 % 10);
		else if constexpr (N == 3) return static_cast<mode>(opcode / 10000 % 10);
		// clang-format on
	}

protected:
	explicit intcode_computer_base(program_type* program) noexcept;
	virtual ~intcode_computer_base();

	static program_type parse_program(std::string const& in);

	program_type* _current_program;
	size_type _ip;
};

class monotask_intcode_computer : public intcode_computer_base {
public:
	static monotask_intcode_computer from_string(std::string const& str);
	monotask_intcode_computer(monotask_intcode_computer const& other);
	monotask_intcode_computer& operator=(monotask_intcode_computer const& other);

protected:
	monotask_intcode_computer(program_type program) noexcept;

	program_type _program;
};

#endif // SRC_INTCODE_HPP
