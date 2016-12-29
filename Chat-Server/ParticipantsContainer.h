#pragma once
#include <vector>
#include <string>

#include "ChatParticipant.h"

class ParticipantsContainer
{
public:
	void add(const std::shared_ptr<ChatParticipant>& participant);
	std::shared_ptr<ChatParticipant> get(const std::string& username);
	void remove(const std::shared_ptr<ChatParticipant>& participant);


	void prune(); // is it useful ???

private:
	std::vector<std::weak_ptr<ChatParticipant>> participants;
};
