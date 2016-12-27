#include "Server.h"
#include "Session.h"
#include "Authenticator.h"


Server::Server(boost::asio::io_service & io_service, short port):
	acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
	socket(io_service), room(std::make_shared<ChatRoom>())
{
	acceptMessages();
}

void Server::acceptMessages()
{
	acceptor.async_accept(socket, [this](boost::system::error_code e)
	{
		if (!e)
		{
			// after std::move socket is in same state as it would be
			// after caling socket(io_service)
			auto auth = std::make_shared<Authenticator>(std::move(socket));
			auth->authenticate([this](auto session)
			{
				session->start();
				session->set_room(room);
			});
		}
		acceptMessages();
	});
}