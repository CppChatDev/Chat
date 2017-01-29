#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <boost/asio.hpp>

#include "User.h"

class Dispatcher;
using boost::asio::ip::tcp;

// Responsible for receiving and sending messages (and requests) to a user
class Session : public User, public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, Dispatcher& dispatcher, std::string username);

	void start();

	// send message, (message id in datbase is specified by second parameter)
	// mesg_id is needed to update database (set message status as send)
	void deliver(const Message& msg, std::string msg_id) override;

private:
	void do_read();
	void do_write();

	// delivers messages which where send to THIS user while he was offline
	// this messages are in database and have status "unsend"
	void deliver_pending();	

	Dispatcher& dispatcher;
	tcp::socket session_socket;
	std::queue<std::pair<Message, std::string>> msg_queue;	// holds queue of {message, message_id} 

	Message read_msg;										// buffer for incoming messages
};