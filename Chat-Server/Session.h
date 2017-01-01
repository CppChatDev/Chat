#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <boost/asio.hpp>

#include "ChatParticipant.h"

class Dispatcher;
using boost::asio::ip::tcp;

class Session : public ChatParticipant, public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, Dispatcher& dispatcher, std::string username);

	void start();
	void deliver(const Message& msg, std::string msg_id) override;

private:
	void do_read();
	void do_write();
	void deliver_pending();	

	Dispatcher& dispatcher;
	tcp::socket session_socket;
	std::queue<std::pair<Message, std::string>> msg_queue; // holds queue of {message, message_id} 

	Message read_msg;
};