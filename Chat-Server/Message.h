#pragma once

#include <vector>
#include <boost/asio.hpp>

class Message
{
public:
	Message(size_t size);
	std::vector<char>& data();

private:
	std::vector<char> data_;
};
