#pragma once
#include <string>

class ChatParticipant
{
public:
	virtual ~ChatParticipant() {}
	virtual void deliver(const std::vector<char>& msg) = 0;
};
