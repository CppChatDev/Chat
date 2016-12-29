#include "Session.h"
#include "DataParser.h"

Session::Session(tcp::socket socket, std::string username) :
	ChatParticipant(move(username)),
	session_socket(std::move(socket)),
	buffer(buffer_size)
{

}

Session::~Session()
{
	// TODO - remove reference to this session from Container
}

void Session::start()
{
	do_read();

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
	buffer.resize(buffer_size);
	session_socket.async_read_some(boost::asio::buffer(buffer),
		[this, self](boost::system::error_code ec, size_t length)
	{
		if (!ec)
		{
			buffer.resize(length);

			//auto code = DataParser::parse_data(buffer);
			//if (code == DataParser::code_type::non_control)
			//{

			//}
			//else if (code == DataParser::code_type::exit)
			//{
			//	end();
			//}
			//else
			//{
			//	// TODO: handle codes
			//	do_read();
			//}

			// raw buffer is not needed anymore, after std::move vector is reusable
			Message msg(move(buffer));

			auto recipient = msg.get_header();	// get recipient of the message
			msg.set_header(username);			// set senderof the message 

			// send message to the recipient
			// TODO

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
			// TODO
		}
	});
}
