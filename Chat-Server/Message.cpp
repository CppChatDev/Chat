#include "Message.h"

Message::Message(size_t size) : data_(size)
{

}

std::vector<char>& Message::data()
{
	return data_;
}
