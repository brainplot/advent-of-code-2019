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
	enum class action : std::uint8_t;
	enum class mode : std::uint8_t;

	constexpr value_type get_parameter(size_type pos, mode m) const noexcept;

	void binary_op(value_type (*op)(value_type, value_type)) noexcept;
	void input();
	void output();
	void jump(bool (*cond)(value_type)) noexcept;

	virtual value_type input_handler() = 0;
	virtual void output_handler(value_type data) = 0;

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
	explicit intcode_base(program_type program) noexcept;
	virtual ~intcode_base();

	static program_type parse_program(std::string const& in);

	program_type _program;
	size_type _ip;
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
