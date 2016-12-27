#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <boost/asio.hpp>

#include "ChatParticipant.h"
#include "ChatRoom.h"

using boost::asio::ip::tcp;

class Session : public ChatParticipant, public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, std::string username);

	void start();
	void deliver(const Message& msg) override;

	// for testing ...., changing rooms should probably occur inside session
	void set_room(std::shared_ptr<ChatRoom> room) { this->room = room; room->join(shared_from_this()); }

private:
	void do_read();
	void do_write();
	void end();

	tcp::socket session_socket;
	std::queue<Message> msg_queue;
	std::shared_ptr<ChatRoom> room;

	const size_t buffer_size = 1024;
	std::vector<char> buffer;
};