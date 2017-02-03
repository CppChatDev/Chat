#include "Dispatcher.h"
#include <boost/date_time/posix_time/posix_time.hpp>

Dispatcher::Dispatcher() : database("database.db")
{
}

void Dispatcher::send(const Message& msg, std::string recipient_name)
{
	// get current time
	auto date = to_simple_string(boost::posix_time::second_clock::local_time());

	std::lock_guard<std::mutex> lock(db_mutex);

	database.execute("INSERT INTO messages (sender_id, recipient_id, date, message, delivered)	\
		VALUES (																				\
		(SELECT id FROM users WHERE username = ? ),												\
		(SELECT id FROM users WHERE username = ? ),												\
		 ?, ?, ? );",
		{msg.get_header(), recipient_name, date, msg.get_str(), "0"});

	// get id of last inserted row in "messages"
	auto msg_id = database.execute("SELECT seq FROM sqlite_sequence WHERE name = \"messages\"")[0]["seq"];

	auto recipient = get_participant(recipient_name); // TODO - should it be under mutex?
	if (recipient != nullptr) // if recipient is online then deliver the message
	{
		recipient->deliver(msg, msg_id);
	}
}

void Dispatcher::add_participant(const std::shared_ptr<User>& participant)
{
	participants.push_back(participant);
}

void Dispatcher::prune()
{
	for (auto it = participants.begin(); it != participants.end();)
	{
		if (it->expired())
			participants.erase(it);
		else
			++it;
	}
}

Database& Dispatcher::get_db()
{
	return database;
}

std::shared_ptr<User> Dispatcher::get_participant(std::string username)
{
	auto participant =
		std::find_if(participants.begin(), participants.end(), [&username](const std::weak_ptr<User>& x) {
			if (auto shared = x.lock())
				if (shared->get_username() == username)
					return true;
			return false;
		});

	if (participant != participants.end())
		return participant->lock();
	else
		return nullptr;
}
