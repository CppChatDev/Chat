#pragma once
#include <vector>

class DataParser
{
public:
	enum class code_type
	{
		non_control = '0',
		empty = '1',
		list_rooms = '2',
		join_room = '3',
		leave_room = '4',
		create_room = '5',
		// ...
		exit = '9'
	};

	static code_type parse_data(std::vector<char>& data);

	static std::vector<char> make_command(code_type code);

private:
	const static char escape_char = '/' ;
};
