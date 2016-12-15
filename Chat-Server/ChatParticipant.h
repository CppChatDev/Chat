#pragma once
#include <string>

#include "Message.h"

class ChatParticipant
{
public:
	virtual ~ChatParticipant() {}
	virtual void deliver(const Message& msg) = 0;
};
