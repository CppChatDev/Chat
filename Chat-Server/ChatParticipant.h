#pragma once
#include <string>

#include "Message.h"

class ChatParticipant
{
public:
	virtual ~ChatParticipant() {}

	const std::vector<char>& get_username() const;
	virtual void deliver(const Message& msg) = 0;

protected:
	std::vector<char> username;
};
