#include "intcode.hpp"

#include <array>

class intcode_computer7 : public intcode_computer {
public:
	static intcode_computer7 from_string(std::string const& in)
	{
		return intcode_computer7(parse_program(in));
	}

	value_type input_handler() override
	{
		auto const res = _should_provide_signal ? _signals[_current_signal] : _previous_output;
		_should_provide_signal = !_should_provide_signal;

		return res;
	}

	void output_handler(value_type const data) override
	{
		_previous_output = data;
	}

	value_type run()
	{
		auto max = std::numeric_limits<value_type>::min();

		do {
			for (_current_signal = 0; _current_signal < _signals.size(); ++_current_signal) {
				auto backup(_program);
				intcode_computer::run();
				if (_previous_output > max)
					max = _previous_output;
				_program = std::move(backup);
				_ip = 0;
			}
			_previous_output = 0;
			_should_provide_signal = true;
		} while (std::next_permutation(_signals.begin(), _signals.end()));

		return max;
	}

private:
	intcode_computer7(program_type program) noexcept
	    : intcode_computer(std::move(program))
	    , _previous_output(0)
	    // , _signals{1,0,4,3,2}
	    , _signals{0, 1, 2, 3, 4}
	    , _current_signal(0)
	    , _should_provide_signal(true)
	{
	}

	value_type _previous_output;
	std::array<std::uint8_t, 5> _signals;
	std::uint8_t _current_signal;
	bool _should_provide_signal;
};

int main(int const argc, char const** const argv)
{
	auto icomputer = intcode_computer7::from_string(load_file(bootstrap(argc, argv)));

	std::cout << icomputer.run() << '\n';
}
