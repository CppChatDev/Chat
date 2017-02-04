#pragma once

#include <boost/asio.hpp>

#include "Dispatcher.h"

using boost::asio::ip::tcp;

class Server
{
public:
	Server(boost::asio::io_service& io_service, short port);

private:
	void accept_messages();

	tcp::acceptor acceptor;
	tcp::socket socket;
	Dispatcher dispatcher;
};
