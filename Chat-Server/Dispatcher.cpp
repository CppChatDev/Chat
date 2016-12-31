#include "Dispatcher.h"
#include <boost/date_time/posix_time/posix_time.hpp>

Dispatcher::Dispatcher() : database("database.db")
{
}

void Dispatcher::send(const Message& msg, std::string recipient_name)
{
	auto date		= to_simple_string(boost::posix_time::second_clock::local_time());
	auto delivered	= "0";

	std::lock_guard<std::mutex> lock(db_mutex);
	auto recipient = get_participant(recipient_name);				// should it be under mutex?
	if (recipient != nullptr)	// recipient is online
	{
		recipient->deliver(msg);
		delivered = "1";
	}

	database.execute("INSERT INTO messages (sender_id, recipient_id, date, message, delivered)	\
		VALUES (																				\
		(SELECT id FROM users WHERE username = ? ),												\
		(SELECT id FROM users WHERE username = ? ),												\
		 ?, ?, ? );",
		{ msg.get_header(), recipient_name, date, msg.get_str(), delivered });

}

void Dispatcher::add_participant(const std::shared_ptr<ChatParticipant>& participant)
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

std::shared_ptr<ChatParticipant> Dispatcher::get_participant(std::string username)
{
	auto participant = std::find_if(participants.begin(), participants.end(),
		[&username](const std::weak_ptr<ChatParticipant>& x)
	{
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
