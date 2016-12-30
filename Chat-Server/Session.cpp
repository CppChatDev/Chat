#include "Session.h"
#include "Database.h"
#include "Dispatcher.h"

Session::Session(tcp::socket socket, Dispatcher& dispatcher, std::string username) :
	ChatParticipant(move(username)),
	dispatcher(dispatcher),
	session_socket(std::move(socket))
{
	
}

Session::~Session()
{
	// how to deal with dangling weak pointers in dispatcher?
	// call Dispatcher.prune()?

	Database database("database.db");
	// save messages that are still in write_queue
}

void Session::start()
{
	dispatcher.add_participant(shared_from_this());
	do_read();

	Database database("database.db");
	// TODO - send all pending messages
}

void Session::deliver(const Message& msg)
{
	auto write_in_progress = !msg_queue.empty();
	msg_queue.push(msg);
	if (!write_in_progress)
		do_write();
}

void Session::do_read()
{
	auto self(shared_from_this());
	session_socket.async_read_some(read_msg.mutable_buffer(),
		[this, self](boost::system::error_code ec, size_t length)
	{
		if (!ec)
		{
			read_msg.set_size(length);

			auto recipient = read_msg.get_header();	// get recipient of the message

			// send message to the recipient
			dispatcher.send(read_msg, recipient);

			do_read();
		}
		else
		{
			// TODO
		}
	});
}

void Session::do_write()
{
	auto self(shared_from_this());
	boost::asio::async_write(session_socket, msg_queue.front().const_buffer(),
		[this, self](boost::system::error_code ec, size_t length)
	{
		if (!ec)
		{
			msg_queue.pop();
			if (!msg_queue.empty())
			{
				do_write();
			}
		}
		else
		{
			// TODO
		}
	});
}
