#include "Dispatcher.h"

Dispatcher::Dispatcher() : database("database.db")
{
}

void Dispatcher::send(Message msg, std::string recipient_name)
{
	auto date = "TODO";
	auto recipient = get_participant(recipient_name);
	auto insert_message = "INSERT INTO messages (sender_id, date, message) VALUES (" +
		msg.get_header() +
		date +
		msg.get_data();

	auto query = "INSERT INTO user_msg (recipient_id, message_id, delivered) VALUES (\
		(SELECT id FROM users WHERE username = " + recipient_name;

	if (recipient != nullptr)	// recipient is online
	{
		recipient->deliver(msg);
	}
	else
	{
		database.execute("");
	}
}

void Dispatcher::add_participant(const std::shared_ptr<ChatParticipant>& participant)
{
	participants.push_back(participant);
}

void Dispatcher::remove_participant(const std::shared_ptr<ChatParticipant>& participant)
{
	std::remove_if(participants.begin(), participants.end(),
		[&participant](const std::weak_ptr<ChatParticipant>& x)
	{
		return participant == x.lock();
	});
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
