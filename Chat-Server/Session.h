#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <boost/asio.hpp>

#include "ChatParticipant.h"
#include "ChatRoom.h"

using boost::asio::ip::tcp;
using Msg = std::vector<char>;
using MsgQueue = std::queue<Msg>;

class Session : public ChatParticipant, public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, ChatRoom& room);

	void start();
	void deliver(const Msg& msg) override;

private:
	void do_read();
	void do_write();

	tcp::socket session_socket;
	MsgQueue msg_queue;
	Msg data;
	ChatRoom& room;
};