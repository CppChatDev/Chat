#include "Message.h"

Message::Message()
{
	data.resize(1024);
	size = 0;
}

Message::Message(data_type other_data) : data(move(other_data))
{
	// TODO - how to handle too big data?
	size = this->data.size();
	this->data.resize(1024);
}

data_type Message::get_header() const
{
	auto header_end = data.find(header_separator);
	if (header_end >= size || header_end == std::string::npos)
		throw std::runtime_error("no header");

	return data.substr(0, header_end);
}

void Message::set_header(data_type header)
{
	// TODO extend set header to cases when header is not yet in the message

	auto header_end = data.find(header_separator);
	auto new_size = header.size() + size - header_end;
	if (new_size >= 1024)
		throw std::runtime_error("header too large");

	data.replace(0, header_end, header);
	data.resize(1024);
	set_size(new_size);
}

void Message::set_size(size_t new_size)
{
	this->size = new_size;

	// add null, so that calling c_str() on data only returns sequnce from 0 to size
	data[new_size] = '\0';
}

const char* Message::get_str() const
{
	return data.c_str();
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
