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
	~Session();

	void start();
	void deliver(const Message& msg) override;


private:
	void do_read();
	void do_write();

	Dispatcher& dispatcher;
	tcp::socket session_socket;
	std::queue<Message> msg_queue; 
	// TODO - save msg_queu to db if connection closes and there are pending messages

	Message read_msg;
};