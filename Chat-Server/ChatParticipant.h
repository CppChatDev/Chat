#pragma once
#include <string>

#include "Message.h"

class ChatParticipant
{
public:
	ChatParticipant();
	virtual ~ChatParticipant() {}

	int get_id() const;
	const std::vector<char>& get_username() const;
	virtual void deliver(const Message& msg) = 0;

protected:
	std::vector<char> username;
	const int id;

	static int static_id;
};
