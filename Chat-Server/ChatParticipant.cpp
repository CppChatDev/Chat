#include "ChatParticipant.h"

ChatParticipant::ChatParticipant(std::string username): username(move(username))
{
}

const std::string& ChatParticipant::get_username() const
{
	return username;
}
