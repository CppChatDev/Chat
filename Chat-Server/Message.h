#pragma once

#include <vector>
#include <boost/asio.hpp>

class Message
{
public:
	enum class code_type
	{
		undefined,
		exit,
		login
		// ...
	};

	// create message, specifing it's type (code) and data
	Message(code_type code, std::vector<char> data);

	//create message from raw data (data read from network)
	Message(std::vector<char> data);

	// get buffer to data
	boost::asio::const_buffers_1 to_buffers();

	code_type get_code() const;

private:
	const char separator = ' ';

	std::vector<char> data;
	code_type code;
};
