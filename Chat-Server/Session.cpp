#include "Session.h"
#include "DataParser.h"

Session::Session(tcp::socket socket, std::string username):
	ChatParticipant(move(username)), session_socket(std::move(socket)),
	room(nullptr), buffer(buffer_size)
{

}

void Session::start()
{
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
	buffer.resize(buffer_size);
	session_socket.async_read_some(boost::asio::buffer(buffer),
		[this, self](boost::system::error_code ec, size_t length)
	{
		if (!ec)
		{
			buffer.resize(length);

			auto code = DataParser::parse_data(buffer);
			if(code == DataParser::code_type::non_control)
			{
				// raw buffer is not needed anymore, after std::move vector is reusable
				Message msg(move(buffer)); 

				room->deliver(msg, self);
				do_read();
			}
			else if(code == DataParser::code_type::exit)
			{
				end();
			}
			else
			{
				// TODO: handle codes
				do_read();
			}
		}
		else
		{
			end();
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
			end();
		}
	});
}

void Session::end()
{
	if(room != nullptr)
		room->leave(shared_from_this());
	session_socket.close();
}
