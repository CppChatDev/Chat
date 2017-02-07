#pragma once
#include <boost/asio.hpp>
#include <string>
#include <utility>

#include "Database.h"
#include "Registration.h"
#include "Session.h"

using boost::asio::ip::tcp;

// Responsible for user authentication
// If authentication succeeds, it creates new instance of 'Session' and calls on_succes function
// with this newly created Session as parameter
// Contructor receives ref to dispatcher because Session's constructor takes it as a prameter
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
