#pragma once
#include <string>
#include <mutex>

#include "Message.h"
#include "Session.h"
#include "Database.h"

// Holds list of all active users and keeps connection to database
// 
class Dispatcher
{
public:
	Dispatcher();

	// send message to a certain user
	void send(const Message& msg, std::string recipient);

	// add new user to list
	void add_participant(const std::shared_ptr<User>& participant); 

	// remove unactive users from list
	void prune();			

	// get connection to database
	Database& get_db();

private:
	std::shared_ptr<User> get_participant(std::string username);
	
	std::vector<std::weak_ptr<User>> participants;
	Database database;
	std::mutex db_mutex;
};