#pragma once
#include <vector>
#include <memory>

#include "ChatParticipant.h"

using ChatParticipantRef = std::shared_ptr<ChatParticipant>;

class ChatRoom
{
public:
	void join(ChatParticipantRef participant);
	void leave(ChatParticipantRef participant);
	void deliver(std::vector<char> msg);

private:
	std::vector<ChatParticipantRef> participants;
};
