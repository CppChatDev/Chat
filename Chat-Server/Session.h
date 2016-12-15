#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <boost/asio.hpp>

#include "ChatParticipant.h"
#include "ChatRoom.h"

using boost::asio::ip::tcp;
using MsgQueue = std::queue<Message>;

class Session : public ChatParticipant, public std::enable_shared_from_this<Session>
{
public:
	size_t max_msg_length = 1024;
	Session(tcp::socket socket, ChatRoom& room);

	void start();
	void deliver(const Message& msg) override;

private:
	void do_read();
	void do_write();

	tcp::socket session_socket;
	MsgQueue msg_queue;
	Message msg;
	ChatRoom& room;
};