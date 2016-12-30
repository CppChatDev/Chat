#include "Message.h"

Message::Message()
{
	size = 0;
}

std::string Message::get_header() const
{
	auto header_end =  std::find(data, data + size, header_separator);
	return std::string(data, header_end);
}

void Message::set_size(size_t size)
{
	this->size = size;
	data[size] = 0;		// add null, so that calling get_str() on data
						// only returns sequnce from 0 to size
}

const char* Message::get_str() const
{
	return data;
}

boost::asio::const_buffers_1 Message::const_buffer() const
{
	boost::asio::const_buffer buffer = boost::asio::buffer(&data[0], size);
	return boost::asio::const_buffers_1(buffer);
}

boost::asio::mutable_buffers_1 Message::mutable_buffer()
{
	boost::asio::mutable_buffer buffer = boost::asio::buffer(&data[0], 1024);
	return boost::asio::mutable_buffers_1(buffer);
}