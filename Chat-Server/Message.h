#pragma once

#include <vector>
#include <boost/asio.hpp>

using data_type = std::string;

class Message
{
public:
	Message();
	Message(data_type data);

	data_type get_header() const;
	void set_header(data_type header);
	void set_size(size_t size);

	const char* get_str() const;
	boost::asio::const_buffers_1 const_buffer() const;
	boost::asio::mutable_buffers_1 mutable_buffer();

private:
	data_type data;
	size_t size = 0;

	const char header_separator = '@';
};


