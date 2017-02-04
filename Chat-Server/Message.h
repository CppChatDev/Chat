#pragma once

#include <boost/asio.hpp>
#include <vector>

using data_type = std::string;

// Message is stored as a std::string and consists of header and message
// It is arranged as header@message (where @ is separator, defined by header_separator const)
class Message
{
public:
	Message();
	Message(data_type data);

	data_type get_header() const;
	void set_header(data_type header);

	// to avoid resizing the buffer, we just keep the current size
	// this method also puts null at data[size] so that if we want c_str
	// we get proper, null-terminated c-string
	void set_size(size_t size);

	// returns null-terminated c-string
	const char* get_str() const;

	// returns asio buffers (used in async_read/write)
	boost::asio::const_buffers_1 const_buffer() const;
	boost::asio::mutable_buffers_1 mutable_buffer();

private:
	data_type data;
	size_t size = 0;

	const char header_separator = '@';
};
