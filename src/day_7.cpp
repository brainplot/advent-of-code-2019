#include "intcode.hpp"

#include <stack>

class amplifier : public intcode_base {
public:
	using phase = std::uint8_t;

	amplifier(program_type program, phase const ph, amplifier* const output_amp)
	    : intcode_base{std::move(program)}
	    , _input_stream{}
	    , _output_stream{output_amp ? &output_amp->_input_stream : nullptr}
	{
		_input_stream.push(ph);
	}

private:
	using stream_type = std::stack<value_type, std::vector<value_type>>;

	value_type input_handler() override
	{
		auto const res = _input_stream.top();
		_input_stream.pop();

		return res;
	}

	void output_handler(value_type const output) override
	{
		if (_output_stream)
			_output_stream->push(output);
		else
			std::cout << output << '\n';
	}

	stream_type _input_stream;
	stream_type* const _output_stream;
};

int main(/*int const argc, char const** const argv*/)
{
	// auto const program_str = load_file(bootstrap(argc, argv));

	// auto circuit = normal_amplification_circuit::from_string(program_str);
	// std::cout << circuit.run() << '\n';

	// auto fl = feedback_loop::from_string(program_str);
	// std::cout << fl.run() << '\n';
}
