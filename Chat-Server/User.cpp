#include "User.h"

User::User(std::string other_username) : username(move(other_username))
{
}

const std::string& User::get_username() const
{
	return username;
}
