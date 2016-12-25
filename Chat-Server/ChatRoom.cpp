#include "ChatRoom.h"
#include <iostream>

int ChatRoom::static_id = 1;

ChatRoom::ChatRoom() : database("database.db"), id(static_id++)
{
}

void ChatRoom::join(ChatParticipantRef participant)
{
	// moving shared_pointer does not change reference count
	// calling push_back is also valid (but slower), because then ref count is incremented 
	// and when particiapant goes out of scope(end of function) ref count is decremented
	participants.emplace_back(participant);
}

void ChatRoom::leave(ChatParticipantRef participant)
{
	participants.erase(find(participants.begin(), participants.end(), participant));
}

void ChatRoom::deliver(const Message& msg, ChatParticipantRef sender)
{
	for (auto &participant : participants)
	{
		if(participant != sender)
			participant->deliver(msg);
	}

	store(msg, sender->get_id());
}

void ChatRoom::store(const Message& msg, int sender_id)
{
	auto data = std::string(msg.get_data().begin(), msg.get_data().end());

	try
	{
		database.execute("INSERT INTO Messages (room_id, sender_id, message) VALUES ("
			+ std::to_string(id) + " , " + std::to_string(sender_id) + " , \"" + data + "\");");
	}
	catch(std::exception& e)
	{
		std::cerr << e.what();
	}
}
