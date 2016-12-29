#include "ParticipantsContainer.h"

void ParticipantsContainer::add(const std::shared_ptr<ChatParticipant>& participant)
{
	participants.push_back(participant);
}

std::shared_ptr<ChatParticipant> ParticipantsContainer::get(const std::string& username)
{
	auto participant = std::find_if(participants.begin(), participants.end(),
		[&username](const std::weak_ptr<ChatParticipant>& x)
	{
		if(auto shared = x.lock())
			if (shared->get_username() == username)
				return true;
		return false;
	});

	if (participant != participants.end())
		return participant->lock();
	else
		return nullptr;
}

void ParticipantsContainer::remove(const std::shared_ptr<ChatParticipant>& participant)
{
	std::remove_if(participants.begin(), participants.end(),
		[&participant](const std::weak_ptr<ChatParticipant>& x)
	{
		return participant == x.lock();
	});
}

void ParticipantsContainer::prune()
{
	for (auto p = participants.begin(); p != participants.end();)
	{
		if (p->expired())
			participants.erase(p);
		else
			++p;
	}
}
