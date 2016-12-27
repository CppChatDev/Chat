#pragma once
#include <string>

#include "Message.h"

class ChatParticipant
{
public:
	ChatParticipant(std::string username);
	virtual ~ChatParticipant() {}

	const std::string& get_username() const;
	virtual void deliver(const Message& msg) = 0;

protected:
	std::string username;
};
