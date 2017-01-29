#pragma once
#include <string>

#include "Message.h"

// Base class for user
class User
{
public:
	explicit User(std::string username);
	virtual ~User() {}

	const std::string& get_username() const;
	virtual void deliver(const Message& msg, std::string msg_id) = 0;

protected:
	std::string username;
};
