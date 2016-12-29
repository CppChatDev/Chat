#include "ChatRoom.h"
#include <iostream>

ChatRoom::ChatRoom() : database("database.db")
{

}

void ChatRoom::join(ParticipantRef participant)
{
	// moving shared_pointer does not change reference count
	// calling push_back is also valid (but slower), because then ref count is incremented 
	// and when particiapant goes out of scope(end of function) ref count is decremented
	participants.emplace_back(participant);
}

void ChatRoom::leave(ParticipantRef participant)
{
	participants.erase(find(participants.begin(), participants.end(), participant));
}

void ChatRoom::deliver(const Message& msg, ParticipantRef sender)
{
	for (auto &participant : participants)
	{
		if(participant != sender)
			participant->deliver(msg);
	}

	//store(msg, sender);
}

void ChatRoom::store(const Message& msg, ParticipantRef& sender)
{
	auto data = std::string(msg.get_data().begin(), msg.get_data().end());
	auto sender_id = database.execute(
		"SELECT id FROM Clients WHERE username = \"" + sender->get_username() + "\"")[0]["id"];

	try
	{
		// TODO - room_id
		database.execute("INSERT INTO Messages (room_id, sender_id, message) VALUES ("
			+ std::to_string(1) + " , " + sender_id + " , \"" + data + "\");");
	}
	catch(std::exception& e)
	{
		std::cerr << e.what();
	}
}
