#include "Message.h"


Message::Message(code_type code, std::vector<char> data): data(data), code(code)
{
}

Message::Message(std::vector<char> data): data(data)
{
	// get index of separator, message format is: code^data where ^ is separator
	auto sep_index = find(data.begin(), data.end(), separator);

	if(sep_index == data.end())
	{
		code = code_type::undefined;
	}
	else
	{
		std::string code_str(data.begin(), sep_index);
		code = static_cast<code_type>(stoi(code_str));
		data.erase(data.begin(), sep_index);

		// TODO - stronger check, catch invalid_argument exception
	}
}

boost::asio::const_buffers_1 Message::to_buffers()
{
	boost::asio::const_buffer buffer = boost::asio::buffer(data);
	return boost::asio::const_buffers_1(buffer);
}

Message::code_type Message::get_code() const
{
	return code;
}
