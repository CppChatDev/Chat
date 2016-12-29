#pragma once
#include <vector>
#include <memory>

#include "ChatParticipant.h"
#include "Database.h"

using ParticipantRef = std::shared_ptr<ChatParticipant>;

class ChatRoom
{
public:
	ChatRoom();
	void join(ParticipantRef participant);
	void leave(ParticipantRef participant);
	void deliver(const Message& msg, ParticipantRef sender);

private:
	void store(const Message& msg, ParticipantRef& sender);

	std::vector<ParticipantRef> participants;
	Database database;
};
