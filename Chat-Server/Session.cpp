#include "Session.h"

Session::Session(tcp::socket socket, ChatRoom& room)
	: session_socket(std::move(socket)), room(room), buffer(buffer_size)
{

}

void Session::start()
{
	room.join(shared_from_this());
	do_read();
}

void Session::deliver(const Message& msg)
{
	msg_queue.push(msg);
	do_write();
}

void Session::do_read()
{
	auto self(shared_from_this());
	session_socket.async_read_some(boost::asio::buffer(buffer),
		[this, self](boost::system::error_code ec, size_t length)
	{
		if (!ec)
		{
			buffer.resize(length);
			Message msg(move(buffer)); 
			// raw buffer is not needed anymore, after std::move vector is reusable

			//TODO - handle message
			if(msg.get_code() == Message::code_type::exit)
			{
				room.leave(self);
				session_socket.close();
				return;
			}
			else
			{
				room.deliver(msg);

				do_read();
			}
		}
		else
		{
			room.leave(self);
		}
	});
}

void Session::do_write()
{
	auto self(shared_from_this());
	boost::asio::async_write(session_socket, msg_queue.front().to_buffers(),
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
			room.leave(self);
		}
	});
}

