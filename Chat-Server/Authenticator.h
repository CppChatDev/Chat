#pragma once
#include <boost/asio.hpp>

#include "Database.h"
#include "Session.h"

using boost::asio::ip::tcp;

class Authenticator
{
public:
	Authenticator(tcp::socket socket);

	std::shared_ptr<Session> make_session();

private:
	tcp::socket socket;
	Database database;
};