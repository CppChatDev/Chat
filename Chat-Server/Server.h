#pragma once

#include <boost/asio.hpp>

#include "ChatRoom.h"
#include "ParticipantsContainer.h"
#include "Dispatcher.h"

using boost::asio::ip::tcp;

class Server
{
public:
	Server(boost::asio::io_service &io_service, short port);

private:
	void acceptMessages();

	tcp::acceptor acceptor;
	tcp::socket socket;
	Dispatcher dispatcher; // TODO - pass reference to session
};

