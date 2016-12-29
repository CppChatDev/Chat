#pragma once
#include <string>

#include "Message.h"
#include "ParticipantsContainer.h"
#include "Session.h"

// supposed to be proxy class which handle messages
// it sends message directly to the recipient when he is online
// or it saves it to database if he's not

// TODO
// however it would be better if every connection (chatroom) had
// it's own database connection (thread safety)
class Dispatcher
{
public:
	Dispatcher();
	void send(Message msg, std::string recipient);
	void add_participant(const std::shared_ptr<ChatParticipant>& participant);
	void remove_participant(const std::shared_ptr<ChatParticipant>& participant);

private:
	std::shared_ptr<ChatParticipant> get_participant(std::string username);

	std::vector<std::weak_ptr<ChatParticipant>> participants;
	Database database;
};