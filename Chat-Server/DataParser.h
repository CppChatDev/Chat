#pragma once
#include <vector>

class DataParser
{
public:
	enum class code_type
	{
		non_control = '0',
		empty = '1',
		// ...
		exit = '9'
	};

	static code_type parse_data(std::vector<char>& data);

	static std::vector<char> make_command(code_type code);

private:
	const static char escape_char = '/' ;
};
