#include "Dispatcher.h"

Dispatcher::Dispatcher() : database("database.db")
{
}

void Dispatcher::send(const Message& msg, std::string recipient_name)
{
	//auto date = "TODO";
	//auto insert_message = "INSERT INTO messages (sender_id, date, message) VALUES (" +
	//	msg.get_header() +
	//	date +
	//	msg.get_str();

	//auto add_pending_message = "INSERT INTO pending_msg (recipient_id, message_id) VALUES (\
	//	(SELECT id FROM users WHERE username = \"" + recipient_name + "\"),\
	//	(SELECT last_insert_rowid())";


	std::lock_guard<std::mutex> lock(db_mutex);
	auto recipient = get_participant(recipient_name);				// should it be under mutex?
	//database.execute(insert_message);	// store message in db
	if (recipient != nullptr)			// recipient is online
	{
		recipient->deliver(msg);
	}
	else								// recipinet is not online 
	{
	//	database.execute(add_pending_message);
	}
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
