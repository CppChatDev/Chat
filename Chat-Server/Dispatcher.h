#pragma once
#include <string>

#include "Message.h"
#include "Session.h"
#include <mutex>
#include "Database.h"

// TODO
// it would be better if every connection (chatroom) had
// it's own database connection (thread pool ?)
class Dispatcher
{
public:
	Dispatcher();
	void send(const Message& msg, std::string recipient);
	void add_participant(const std::shared_ptr<ChatParticipant>& participant); 
	void prune();

	Database& get_db();

private:
	std::shared_ptr<ChatParticipant> get_participant(std::string username);
	
	std::vector<std::weak_ptr<ChatParticipant>> participants;
	Database database;
	std::mutex db_mutex;
};