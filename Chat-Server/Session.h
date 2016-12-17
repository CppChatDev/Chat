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
	Session(tcp::socket socket, ChatRoom& room);

	void start();
	void deliver(const Message& msg) override;

private:
	void do_read();
	void do_write();

	tcp::socket session_socket;
	std::queue<Message> msg_queue;
	ChatRoom& room;

	const size_t buffer_size = 1024;
	std::vector<char> buffer;
};