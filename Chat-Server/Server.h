#pragma once

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public: 
	session(tcp::socket socket) : ssocket(std::move(socket))
	{
		
	}

	void start()
	{
		do_read();
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		ssocket.async_read_some(boost::asio::buffer(data, 1024), [this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec) do_write(length);
		});
	}

	void do_write(std::size_t length)
	{
		auto self(shared_from_this());
		boost::asio::async_write(ssocket, boost::asio::buffer(data, 1024), [this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec) do_read();
		});
	}

	tcp::socket ssocket;
	char data[1024];
};

class Server
{
public:
	Server(boost::asio::io_service &io_service, short port);

private:
	void acceptMessages();
	void parseMessage();

	tcp::acceptor acceptor;
	tcp::socket socket;
	std::vector<char> message_buffer;
};

