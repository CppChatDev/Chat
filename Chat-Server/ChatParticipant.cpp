#include "ChatParticipant.h"

int ChatParticipant::static_id = 1;

ChatParticipant::ChatParticipant() : id(static_id++)
{

}

int ChatParticipant::get_id() const
{
	return id;
}

const std::vector<char>& ChatParticipant::get_username() const
{
	return username;
}
