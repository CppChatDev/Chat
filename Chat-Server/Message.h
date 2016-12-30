#pragma once

#include <vector>
#include <boost/asio.hpp>

class Message
{
public:
	Message();
	std::string get_header() const;
	void set_size(size_t size);

	const char* get_str() const;
	boost::asio::const_buffers_1 const_buffer() const;
	boost::asio::mutable_buffers_1 mutable_buffer();

private:
	char data[1024];
	size_t size = 0;

	const char header_separator = '@';
};