#include "Message.h"

Message::Message(size_t size) : data(size)
{

}

boost::asio::mutable_buffers_1 Message::buffer()
{
	return boost::asio::buffer(data);
}

boost::asio::mutable_buffers_1 Message::empty_buffer()
{
	// data.clear();
	return boost::asio::buffer(data);
}

size_t Message::length() const
{
	return data.size();
}

