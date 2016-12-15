#pragma once

#include <vector>
#include <boost/asio.hpp>

class Message
{
public:
	Message(size_t size);
	size_t length() const;

	boost::asio::mutable_buffers_1 buffer();
	boost::asio::mutable_buffers_1 empty_buffer();

private:
	std::vector<char> data;
};
