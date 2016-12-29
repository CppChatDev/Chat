#include <algorithm>

#include "Message.h"

// vector is passed by value and moved so it covers 2 cases:
// - when vector is passed by value there is copy constrcutor and then moving constructor involved
// - when vector is passed by r-reference 2 copy constructors are involved
// 2 ctrs - one taking 'const vector&' and one 'vector&&' would be more efficient(but more writing)
Message::Message(std::vector<char> data): data(move(data))
{

}

std::string Message::get_header()
{
	// for now - message format is: header@message
	auto end_it = find(data.begin(), data.end(), '@');
	return std::string(data.begin(), end_it);
}

void Message::set_header(std::string header)
{
	auto end_it = find(data.begin(), data.end(), '@');

	if(end_it != data.end())
		data.erase(data.begin(), end_it);	// delete old header

	data.insert(data.begin(), header.begin(), header.end());
}

boost::asio::const_buffers_1 Message::to_buffers() const
{
	boost::asio::const_buffer buffer = boost::asio::buffer(data);
	return boost::asio::const_buffers_1(buffer);
}

const std::vector<char>& Message::get_data() const
{
	return data;
}
