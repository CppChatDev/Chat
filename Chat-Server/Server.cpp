#include "Server.h"


Server::Server(boost::asio::io_service & io_service, short port) 
	: acceptor(io_service, tcp::endpoint(tcp::v4(), port)), 
	  socket(io_service)
{
	acceptMessages();
}

void Server::acceptMessages()
{
	acceptor.async_accept(socket, [this](boost::system::error_code e)
	{
		if (!e)
		{
			std::make_shared<session>(std::move(socket))->start();
		}
		acceptMessages();
	});
}

void Server::parseMessage()
{
	socket.async_read_some(boost::asio::buffer(message_buffer), [this](boost::system::error_code e, std::size_t length)
	{
		std::cout << "I heard something!\n";
	});
}
