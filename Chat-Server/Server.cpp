#include "Server.h"
#include "Session.h"


Server::Server(boost::asio::io_service & io_service, short port) 
	: acceptor(io_service, tcp::endpoint(tcp::v4(), port)), 
	  socket(io_service)
{
	int result = sqlite3_open("database.db", &db);
	if (result != SQLITE_OK)
	{
		std::cerr << "There was a problem opening the database. Exiting!\n";
		getchar();
		exit(result);
	}
	acceptMessages();
}

void Server::acceptMessages()
{
	acceptor.async_accept(socket, [this](boost::system::error_code e)
	{
		if (!e)
		{
			std::make_shared<Session>(std::move(socket))->start();
		}
		acceptMessages();
	});
}