#include "User.h"

User::User(std::string username): username(move(username))
{
}

const std::string& User::get_username() const
{
	return username;
}
