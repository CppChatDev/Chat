#include "Authenticator.h"

Authenticator::Authenticator(tcp::socket socket) :
	socket(std::move(socket)), database("database.db")
{

}

std::shared_ptr<Session> Authenticator::make_session()
{
	// TODO
	// read username, check password/ssh key 

	return std::make_shared<Session>(std::move(socket), "username");
}
