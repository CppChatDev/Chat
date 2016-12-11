#pragma once

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <sqlite3.h>

using boost::asio::ip::tcp;

class Server
{
public:
	Server(boost::asio::io_service &io_service, short port);

private:
	void acceptMessages();

	tcp::acceptor acceptor;
	tcp::socket socket;
	std::vector<char> message_buffer;
};

