#pragma once
#include <vector>
#include <memory>

#include "ChatParticipant.h"
#include "Database.h"

using ChatParticipantRef = std::shared_ptr<ChatParticipant>;

class ChatRoom
{
public:
	ChatRoom();
	void join(ChatParticipantRef participant);
	void leave(ChatParticipantRef participant);
	void deliver(const Message& msg, ChatParticipantRef sender);

private:
	void store(const Message& msg, int sender_id);

	std::vector<ChatParticipantRef> participants;
	Database database;
	int id;

	static int static_id;
};
