#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

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

