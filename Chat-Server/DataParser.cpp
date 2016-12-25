#include "DataParser.h"

DataParser::code_type DataParser::parse_data(std::vector<char>& data)
{
	if (data.empty())
		return code_type::empty;
	if(data[0] == escape_char)
	{
		data.erase(data.begin(), data.begin() + 1); // remove escape char
		return code_type::non_control;
	}

	return static_cast<code_type>(data[0]);
}

std::vector<char> DataParser::make_command(code_type code)
{
	return{ static_cast<char>(code) };
}
