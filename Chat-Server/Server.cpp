#include "Server.h"
#include "Session.h"
#include "Authenticator.h"


Server::Server(boost::asio::io_service & io_service, short port):
	acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
	socket(io_service)
{
	accept_messages();
}

void Server::accept_messages()
{
	acceptor.async_accept(socket, [this](boost::system::error_code e)
	{
		if (!e)
		{
			try
			{
				// after std::move socket is in same state as it would be
				// after caling socket(io_service)
				auto auth = std::make_shared<Authenticator>(std::move(socket), dispatcher);
				auth->authenticate([this](std::shared_ptr<Session> session)
					// this is on_success method which is called from authenticator
					// here, we have session which was created inside authenticator
				{
					session->start();
				});
			}
			catch(std::exception& ex)
			{
				// TODO - authenticator may throw (database) and session's start method(database)
			}
		}
		accept_messages();
	});
}
