#include "Session.h"

Session::Session(tcp::socket socket, ChatRoom& room) 
	: session_socket(std::move(socket)), data(1024), room(room)
{
	
}

void Session::start()
{
	room.join(shared_from_this());
	do_read();
}

void Session::deliver(const Msg& msg)
{
	msg_queue.push(msg);
	do_write();
}

void Session::do_read()
{
	auto self(shared_from_this());
	session_socket.async_read_some(boost::asio::buffer(data),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			data.resize(length);
			room.deliver(data);

			do_read();
		}
		else
		{
			// TODO
			room.leave(self);
		}
	});
}

void Session::do_write()
{
	auto self(shared_from_this());
	boost::asio::async_write(session_socket, boost::asio::buffer(msg_queue.front()),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			msg_queue.pop();
			if(!msg_queue.empty())
			{
				do_write();
			}
		}
		else
		{
			// TODO
			room.leave(self);
		}
	});
}