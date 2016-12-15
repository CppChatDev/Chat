#include "ChatRoom.h"

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

void ChatRoom::deliver(const Message& msg)
{
	for (auto &participant : participants)
		participant->deliver(msg);
}
