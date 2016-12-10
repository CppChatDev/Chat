#include "Session.h"

void Session::start()
{
	do_read();
}

void Session::do_read()
{
	auto self(shared_from_this());
	session_socket.async_read_some(boost::asio::buffer(data, 1024), [this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec) do_write(length);
	});
}

void Session::do_write(std::size_t length)
{
	auto self(shared_from_this());
	boost::asio::async_write(session_socket, boost::asio::buffer(data, 1024), [this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec) do_read();
	});
}