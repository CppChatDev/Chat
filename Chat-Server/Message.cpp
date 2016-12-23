#include "Message.h"

// vectored is passed by value and moved so it covers 2 cases:
// - when vector is passed by value there is copy constrcutor and then moving constructor involved
// - when vector is passed by r-reference 2 copy constructors are involved
// 2 ctrs - one taking 'const vector&' and one 'vector&&' would be more efficient(but more writing)
Message::Message(code_type code, std::vector<char> data): data(move(data)), code(code)
{
}

Message::Message(std::vector<char> data): data(data)
{
	auto c = static_cast<code_type>(data[0]);
	if (c <= code_type::undefined || c > code_type::exit)
		code = code_type::undefined;
	else
	{
		code = c;
		data.erase(data.begin(), data.begin() + 1);
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
