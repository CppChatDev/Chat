#pragma once
#include <boost/asio.hpp>

#include "Database.h"
#include "Session.h"

using boost::asio::ip::tcp;

class Authenticator : public std::enable_shared_from_this<Authenticator>
{
public:
	Authenticator(tcp::socket socket, Dispatcher& dispatcher);

	void authenticate(std::function<void(std::shared_ptr<Session>)> on_success);

private:
	tcp::socket socket;
	Database database;
	Dispatcher& dispatcher;

	char buffer[1024];
};